// Gemini Languages and Virtual Machine
// Copyright 2021 Aldo Jose Nunez
//
// Licensed under the Apache License, Version 2.0.
// See the LICENSE.txt file for details.

#include "pch.h"
#include "BinderVisitor.h"
#include "FolderVisitor.h"
#include <assert.h>
#include <inttypes.h>
#include <set>
#include <stdio.h>


namespace Gemini
{

class LocalScope
{
    SymTable            mLocalTable;
    BinderVisitor&      mBinder;

public:
    explicit LocalScope( BinderVisitor& binder ) :
        mBinder( binder )
    {
        mBinder.mSymStack.push_back( &mLocalTable );
        mBinder.mCurLevelLocalCount = 0;
    }

    ~LocalScope()
    {
        mBinder.mCurLocalCount -= mBinder.mCurLevelLocalCount;
        mBinder.mCurLevelLocalCount = 0;
        mBinder.mSymStack.pop_back();
    }

    LocalScope( const LocalScope& ) = delete;
    LocalScope& operator=( const LocalScope& ) = delete;
};


class BorrowedScope
{
    SymTable&           mTable;
    BinderVisitor&      mBinder;

public:
    explicit BorrowedScope( BinderVisitor& binder, SymTable& table ) :
        mTable( table ),
        mBinder( binder )
    {
        mBinder.mSymStack.push_back( &mTable );
    }

    ~BorrowedScope()
    {
        mBinder.mSymStack.pop_back();
    }

    BorrowedScope( const BorrowedScope& ) = delete;
    BorrowedScope& operator=( const BorrowedScope& ) = delete;
};


static bool IsFunctionDeclaration( DeclKind kind )
{
    return kind == DeclKind::Func
        ;
}

static bool IsCallableDeclaration( DeclKind kind )
{
    return kind == DeclKind::Func
        || kind == DeclKind::NativeFunc
        ;
}

static bool IsVarDeclaration( DeclKind kind )
{
    return kind == DeclKind::Param
        || kind == DeclKind::Global
        || kind == DeclKind::Local
        || kind == DeclKind::Field
        ;
}

static bool IsSimpleValueDeclaration( DeclKind kind )
{
    return kind == DeclKind::Param
        || kind == DeclKind::Global
        || kind == DeclKind::Local
        || kind == DeclKind::Const
        ;
}

static bool IsAddressableDeclaration( DeclKind kind )
{
    return kind == DeclKind::Func
        ;
}

bool IsScalarType( TypeKind kind )
{
    return kind == TypeKind::Int
        || kind == TypeKind::Enum
        || kind == TypeKind::Pointer
        ;
}

bool IsIntegralType( TypeKind kind )
{
    return kind == TypeKind::Int
        || kind == TypeKind::Enum
        ;
}

static bool IsAddressableType( TypeKind kind )
{
    return kind == TypeKind::Func;
}

static bool IsStatementType( TypeKind kind )
{
    return IsScalarType( kind )
        || kind == TypeKind::Xfer
        ;
}

static bool IsAssignableType( TypeKind kind )
{
    return IsScalarType( kind )
        || kind == TypeKind::Xfer
        || kind == TypeKind::Array
        || kind == TypeKind::Record
        ;
}

static bool IsEquatable( TypeKind kind )
{
    return IsScalarType( kind )
        ;
}

static bool IsBoolean( TypeKind kind )
{
    return IsIntegralType( kind );
}

static bool IsAllowedPointerTarget( TypeKind kind )
{
    return kind == TypeKind::Func;
}

static bool IsAllowedParamType( TypeKind kind, ParamMode mode )
{
    if ( kind == TypeKind::Array
        || kind == TypeKind::Record )
        return mode == ParamMode::InOutRef;

    return IsScalarType( kind )
        ;
}

static bool IsStorageType( TypeKind kind )
{
    return IsScalarType( kind )
        || kind == TypeKind::Array
        || kind == TypeKind::Record
        ;
}

static bool IsLValue( const Syntax& node )
{
    if ( node.Kind != SyntaxKind::Name
        && node.Kind != SyntaxKind::DotExpr
        && node.Kind != SyntaxKind::Index
        && node.Kind != SyntaxKind::Slice )
    {
        return false;
    }

    return IsAssignableType( node.Type->GetKind() );
}

template <typename T, typename... Args>
std::shared_ptr<T> Make( Args&&... args )
{
    T* type = new T( std::forward<Args>( args )... );
    return std::shared_ptr<T>( type );
}


BinderVisitor::BinderVisitor(
    ModSize modIndex,
    SymTable& globalTable,
    SymTable& moduleTable,
    SymTable& publicTable,
    ICompilerLog* log )
    :
    mGlobalTable( globalTable ),
    mModuleTable( moduleTable ),
    mPublicTable( publicTable ),
    mRep( log ),
    mModIndex( modIndex )
{
    mSymStack.push_back( &mGlobalTable );

    MakeStdEnv();
}

void BinderVisitor::Declare( Unit* unit )
{
    for ( auto& varNode : unit->DataDeclarations )
        DeclareNode( varNode.get() );

    for ( auto& funcNode : unit->FuncDeclarations )
        DeclareNode( funcNode.get() );
}

void BinderVisitor::BindDeclarations( Unit* unit )
{
    unit->Accept( this );
}

void BinderVisitor::BindFunctionBodies( Unit* unit )
{
    BindProcs( unit );
    BindLambdas( unit );
}

size_t BinderVisitor::GetDataSize()
{
    return mGlobalSize;
}

void BinderVisitor::VisitAddrOfExpr( AddrOfExpr* addrOf )
{
    Visit( addrOf->Inner );

    const auto& innerType = addrOf->Inner->Type;
    auto decl = addrOf->Inner->GetDecl();

    if ( !innerType || !IsAddressableType( innerType->GetKind() )
        || decl == nullptr || !IsAddressableDeclaration( decl->Kind ) )
    {
        mRep.ThrowSemanticsError( addrOf->Inner.get(), "Expected a function" );
    }

    addrOf->Type = Make<PointerType>( innerType );
}

void BinderVisitor::VisitArrayTypeRef( ArrayTypeRef* typeRef )
{
    DataSize size = 0;

    std::shared_ptr<Type> elemType;

    if ( typeRef->ElementTypeRef )
    {
        typeRef->ElementTypeRef->Accept( this );

        elemType = typeRef->ElementTypeRef->ReferentType;
    }
    else
    {
        elemType = mIntType;
    }

    if ( typeRef->SizeExpr )
    {
        Visit( typeRef->SizeExpr );

        int32_t rawSize = Evaluate( typeRef->SizeExpr.get(), "Expected a constant array size" );

        if ( rawSize <= 0 )
            mRep.ThrowSemanticsError( typeRef->SizeExpr.get(), "Array size must be positive" );

        size = CheckArraySize( static_cast<size_t>(rawSize), elemType.get(), typeRef->SizeExpr.get() );
    }
    // Else, it's an open array

    if ( !IsStorageType( elemType->GetKind() ) )
        mRep.ThrowSemanticsError( typeRef->SizeExpr.get(), "Element type is not allowed" );

    typeRef->Type = mTypeType;
    typeRef->ReferentType = Make<ArrayType>( size, elemType );
}

void BinderVisitor::VisitAsExpr( AsExpr* asExpr )
{
    Visit( asExpr->Inner );

    asExpr->TargetTypeRef->Accept( this );

    auto srcType = asExpr->Inner->Type;
    auto dstType = asExpr->TargetTypeRef->ReferentType;

    if ( !IsIntegralType( srcType->GetKind() ) )
        mRep.ThrowSemanticsError( asExpr->Inner.get(), "Type is not integral" );

    if ( !IsIntegralType( dstType->GetKind() ) )
        mRep.ThrowSemanticsError( asExpr->TargetTypeRef.get(), "Type is not integral" );

    asExpr->Type = dstType;
}

void BinderVisitor::VisitAssignmentExpr( AssignmentExpr* assignment )
{
    Visit( assignment->Left );
    Visit( assignment->Right );

    if ( !IsLValue( *assignment->Left ) )
        mRep.ThrowSemanticsError( assignment->Left.get(), "Left side cannot be assigned" );

    CheckType( assignment->Left->Type, assignment->Right->Type, assignment );

    assignment->Type = assignment->Left->Type;
}

void BinderVisitor::VisitBinaryExpr( BinaryExpr* binary )
{
    Visit( binary->Left );
    Visit( binary->Right );

    if ( binary->Op == "=" || binary->Op == "<>" )
    {
        if ( !IsEquatable( binary->Left->Type->GetKind() )
            || !IsEquatable( binary->Right->Type->GetKind() ) )
        {
            mRep.ThrowSemanticsError( binary, "Equality expressions only support scalars" );
        }

        CheckType( binary->Left->Type, binary->Right->Type, binary );

        binary->Type = mIntType;
    }
    else
    {
        if ( binary->Left->Type->GetKind() != TypeKind::Int
            || binary->Right->Type->GetKind() != TypeKind::Int )
        {
            mRep.ThrowSemanticsError( binary, "Binary expressions only support integers" );
        }

        binary->Type = binary->Left->Type;
    }
}

void BinderVisitor::VisitBreakStatement( BreakStatement* breakStmt )
{
    breakStmt->Type = mXferType;
}

void BinderVisitor::VisitCallExpr( CallExpr* call )
{
    Visit( call->Head );

    std::shared_ptr<FuncType> funcType;

    if ( call->IsIndirect )
    {
        auto type = call->Head->Type.get();

        if ( type == nullptr
            || type->GetKind() != TypeKind::Pointer
            || ((PointerType*) type)->TargetType->GetKind() != TypeKind::Func )
        {
            mRep.ThrowSemanticsError( call->Head.get(), "Expected a function pointer" );
        }

        funcType = std::static_pointer_cast<FuncType>( ((PointerType*) type)->TargetType );
    }
    else
    {
        if ( call->Head->Type->GetKind() != TypeKind::Func )
            mRep.ThrowSemanticsError( call->Head.get(), "Expected a function" );

        funcType = std::static_pointer_cast<FuncType>( call->Head->Type );
    }

    if ( call->Arguments.size() != funcType->Params.size() )
        mRep.ThrowSemanticsError( call, "Function does not take %u arguments", call->Arguments.size() );

    ParamSize i = 0;

    for ( auto& arg : call->Arguments )
    {
        Visit( arg );

        CheckArgument( funcType->Params[i].Mode, funcType->Params[i].Type, arg.get() );
        i++;
    }

    call->Type = funcType->ReturnType;
}

void BinderVisitor::VisitCallOrSymbolExpr( CallOrSymbolExpr* callOrSymbol )
{
    Visit( callOrSymbol->Symbol );

    auto decl = callOrSymbol->Symbol->GetDecl();

    if ( IsCallableDeclaration( decl->Kind ) )
    {
        auto funcType = std::static_pointer_cast<FuncType>( decl->GetType() );

        if ( funcType->Params.size() > 0 )
            mRep.ThrowSemanticsError( callOrSymbol, "Too few arguments" );

        callOrSymbol->Type = funcType->ReturnType;
    }
    else
    {
        callOrSymbol->Type = decl->GetType();
    }
}

void BinderVisitor::VisitCaseExpr( CaseExpr* caseExpr )
{
    Visit( caseExpr->TestKey );

    if ( !IsIntegralType( caseExpr->TestKey->Type->GetKind() ) )
        mRep.ThrowSemanticsError( caseExpr->TestKey.get(), "Case only supports integral types" );

    auto decl = caseExpr->TestKey->GetDecl();

    if ( caseExpr->TestKey->Kind != SyntaxKind::Number
        && (decl == nullptr || !IsSimpleValueDeclaration( decl->Kind )) )
    {
        RewriteCaseWithComplexKey( caseExpr );
        return;
    }

    std::shared_ptr<Type> bodyType;

    for ( auto& clause : caseExpr->Clauses )
    {
        for ( auto& key : clause->Keys )
        {
            Visit( key );

            CheckType( caseExpr->TestKey->Type, key->Type, key.get() );
        }

        clause->Body.Accept( this );

        CheckAndConsolidateClauseType( clause->Body, bodyType );
    }

    if ( caseExpr->Fallback )
    {
        caseExpr->Fallback->Body.Accept( this );

        CheckAndConsolidateClauseType( caseExpr->Fallback->Body, bodyType );
    }
    else if ( caseExpr->Clauses.size() > 0 )
    {
        if ( bodyType->GetKind() != TypeKind::Int
            && bodyType->GetKind() != TypeKind::Xfer )
            mRep.ThrowSemanticsError( caseExpr, "Case without else must yield integers or nothing" );
    }
    else
    {
        bodyType = mIntType;
    }

    caseExpr->Type = bodyType;
}

void BinderVisitor::RewriteCaseWithComplexKey( CaseExpr* caseExpr )
{
    Unique<VarDecl> varDecl{ new VarDecl };
    varDecl->Name = "$testKey";
    varDecl->Initializer = std::move( caseExpr->TestKey );

    Unique<NameExpr> nameExpr{ new NameExpr( "$testKey" ) };

    // Logically, we put the original case inside a new Let block.
    // But the original node is still owned up the tree.
    // So, make a copy of the case expression node.

    Unique<CaseExpr> newCase{ new CaseExpr };
    newCase->Clauses.swap( caseExpr->Clauses );
    newCase->Fallback.swap( caseExpr->Fallback );
    newCase->TestKey = std::move( nameExpr );
    CopyBaseSyntax( *newCase, *caseExpr );

    Unique<LetStatement> letStmt{ new LetStatement };
    letStmt->Variables.push_back( std::move( varDecl ) );
    letStmt->Body.Statements.push_back( std::move( newCase ) );

    letStmt->Accept( this );

    mReplacementNode = std::move( letStmt );
}

void BinderVisitor::VisitCondExpr( CondExpr* condExpr )
{
    std::shared_ptr<Type> bodyType;

    for ( auto& clause : condExpr->Clauses )
    {
        Visit( clause->Condition );

        if ( !IsBoolean( clause->Condition->Type->GetKind() ) )
            mRep.ThrowSemanticsError( clause->Condition.get(), "Expected scalar type" );

        clause->Body.Accept( this );

        Syntax* sourceNode = nullptr;

        if ( clause->Body.Statements.size() == 0 && !condExpr->IsIf )
            sourceNode = clause->Condition.get();
        else
            sourceNode = &clause->Body;

        CheckAndConsolidateClauseType( sourceNode, bodyType );
    }

    if ( condExpr->Clauses.size() > 0 )
    {
        auto optVal = GetOptionalSyntaxValue( condExpr->Clauses.back()->Condition.get() );

        if ( !optVal.has_value() || optVal.value() == 0 )
        {
            if ( bodyType->GetKind() != TypeKind::Int
                && bodyType->GetKind() != TypeKind::Xfer )
            {
                mRep.ThrowSemanticsError( condExpr, "If without else must yield integers or nothing" );
            }
        }
    }
    else
    {
        bodyType = mIntType;
    }

    condExpr->Type = bodyType;
}

void BinderVisitor::VisitConstDecl( ConstDecl* constDecl )
{
    if ( constDecl->Decl )
        return;

    PrepareToDefine( constDecl );

    VisitConstBinding( constDecl, ScopeKind::Global );
}

void BinderVisitor::VisitConstBinding( ConstDecl* constDecl, ScopeKind scopeKind )
{
    if ( !constDecl->Initializer )
        THROW_INTERNAL_ERROR( "Missing constant initializer" );

    Visit( constDecl->Initializer );

    if ( constDecl->TypeRef )
    {
        constDecl->TypeRef->Accept( this );

        CheckType( constDecl->TypeRef->ReferentType, constDecl->Initializer->Type, constDecl->Initializer.get() );
    }

    std::shared_ptr<Type> type = constDecl->Initializer->Type;

    if ( IsIntegralType( type->GetKind() ) )
    {
        int32_t value = Evaluate( constDecl->Initializer.get(), "Constant initializer is not constant" );

        std::shared_ptr<Constant> constant;

        if ( scopeKind == ScopeKind::Global )
            constant = AddConst( constDecl, type, value, true );
        else
            constant = AddConst( constDecl, type, value, *mSymStack.back() );

        constDecl->Decl = constant;
    }
    else
    {
        mRep.ThrowSemanticsError( constDecl, "Only integer constants are supported" );
    }
}

void BinderVisitor::VisitCountofExpr( CountofExpr* countofExpr )
{
    Visit( countofExpr->Expr );

    if ( countofExpr->Expr->Type->GetKind() != TypeKind::Array )
        mRep.ThrowSemanticsError( countofExpr->Expr.get(), "Countof applies to arrays" );

    countofExpr->Type = mIntType;
}

void BinderVisitor::VisitDotExpr( DotExpr* dotExpr )
{
    Visit( dotExpr->Head );

    if ( dotExpr->Head->Type->GetKind() == TypeKind::Module )
    {
        auto decl = dotExpr->Head->GetDecl();

        assert( decl->Kind == DeclKind::Module );

        auto modDecl = (ModuleDeclaration*) decl;

        auto it = modDecl->Table.find( dotExpr->Member );

        if ( it == modDecl->Table.end() )
            mRep.ThrowSemanticsError( dotExpr, "Member not found: %s", dotExpr->Member.c_str() );

        dotExpr->Decl = it->second;
        dotExpr->Type = dotExpr->Decl->GetType();
    }
    else if ( dotExpr->Head->Type->GetKind() == TypeKind::Type )
    {
        auto decl = dotExpr->Head->GetDecl();

        if ( decl == nullptr || decl->Kind != DeclKind::Type
            || ((TypeDeclaration*) decl)->ReferentType->GetKind() != TypeKind::Enum )
        {
            mRep.ThrowSemanticsError( dotExpr->Head.get(), "Expected a named type" );
        }

        auto enumType = (EnumType*) ((TypeDeclaration*) decl)->ReferentType.get();

        auto it = enumType->MembersByName.find( dotExpr->Member );

        if ( it == enumType->MembersByName.end() )
            mRep.ThrowSemanticsError( dotExpr, "Member not found: %s", dotExpr->Member.c_str() );

        dotExpr->Decl = it->second;
        dotExpr->Type = dotExpr->Decl->GetType();
    }
    else if ( dotExpr->Head->Type->GetKind() == TypeKind::Record )
    {
        auto& recType = (RecordType&) *dotExpr->Head->Type;

        auto it = recType.Fields.find( dotExpr->Member );

        if ( it == recType.Fields.end() )
            mRep.ThrowSemanticsError( dotExpr, "Member not found: %s", dotExpr->Member.c_str() );

        dotExpr->Decl = it->second;
        dotExpr->Type = dotExpr->Decl->GetType();
    }
    else
    {
        mRep.ThrowSemanticsError( dotExpr->Head.get(), "Can only access members of a module" );
    }
}

void BinderVisitor::VisitEnumTypeRef( EnumTypeRef* enumTypeRef )
{
    auto enumType = Make<EnumType>();

    BorrowedScope scope( *this, enumType->MembersByName );

    int32_t value = -1;

    for ( auto& memberDef : enumTypeRef->Members )
    {
        CheckDuplicateSymbol( memberDef.get(), enumType->MembersByName );

        if ( memberDef->Initializer )
        {
            Visit( memberDef->Initializer );

            if ( memberDef->Initializer->Type->GetKind() != TypeKind::Int )
                CheckType( enumType, memberDef->Initializer->Type, memberDef->Initializer.get() );

            value = Evaluate( memberDef->Initializer.get() );
        }
        else
        {
            if ( value == INT32_MAX )
                mRep.ThrowSemanticsError( memberDef.get(), "Enum member is out of range" );

            value++;
        }

        auto member = Make<EnumMember>( value, enumType );

        enumType->MembersByName.insert( SymTable::value_type( memberDef->Name, member ) );
    }

    enumTypeRef->Type = mTypeType;
    enumTypeRef->ReferentType = enumType;
}

void BinderVisitor::VisitFieldDecl( FieldDecl* fieldDecl )
{
    std::shared_ptr<Type> type;

    if ( fieldDecl->TypeRef )
    {
        fieldDecl->TypeRef->Accept( this );

        type = fieldDecl->TypeRef->ReferentType;
    }
    else
    {
        type = mIntType;
    }

    fieldDecl->Type = type;
}

void BinderVisitor::VisitForStatement( ForStatement* forStmt )
{
    LocalScope localScope( *this );

    auto local = AddLocal( forStmt->Index.get(), mIntType, 1 );

    forStmt->IndexDecl = local;

    Visit( forStmt->First );
    Visit( forStmt->Last );

    if ( forStmt->Step )
        Visit( forStmt->Step );

    forStmt->Body.Accept( this );

    if ( forStmt->First->Type->GetKind() != TypeKind::Int )
        mRep.ThrowSemanticsError( forStmt->First.get(), "For bounds only support integers" );

    if ( forStmt->Last->Type->GetKind() != TypeKind::Int )
        mRep.ThrowSemanticsError( forStmt->Last.get(), "For bounds only support integers" );

    if ( forStmt->Step )
    {
        if ( forStmt->Step->Type->GetKind() != TypeKind::Int )
            mRep.ThrowSemanticsError( forStmt->Step.get(), "For step only supports integers" );
    }

    forStmt->Type = mIntType;
}

void BinderVisitor::VisitImportDecl( ImportDecl* importDecl )
{
    if ( importDecl->Decl )
        return;

    PrepareToDefine( importDecl );

    auto it = mModuleTable.find( importDecl->OriginalName );

    if ( it == mModuleTable.end() )
        mRep.ThrowSemanticsError( importDecl, "Module not found" );

    AddModule( importDecl, std::static_pointer_cast<ModuleDeclaration>( it->second ) );

    importDecl->Decl = it->second;
}

void BinderVisitor::VisitIndexExpr( IndexExpr* indexExpr )
{
    Visit( indexExpr->Head );
    Visit( indexExpr->Index );

    if ( indexExpr->Head->Type->GetKind() != TypeKind::Array )
        mRep.ThrowSemanticsError( indexExpr->Head.get(), "Only arrays can be indexed" );

    if ( indexExpr->Index->Type->GetKind() != TypeKind::Int )
        mRep.ThrowSemanticsError( indexExpr->Index.get(), "Index only supports integers" );

    auto arrayType = (ArrayType*) indexExpr->Head->Type.get();

    if ( arrayType->Count > 0 )
    {
        std::optional<int32_t> optIndexVal = GetOptionalSyntaxValue( indexExpr->Index.get() );

        if ( optIndexVal.has_value() )
        {
            if ( optIndexVal.value() < 0 || optIndexVal.value() >= arrayType->Count )
                mRep.ThrowSemanticsError( indexExpr->Index.get(), "Index must be within bounds of array" );
        }
    }

    indexExpr->Type = arrayType->ElemType;
}

void BinderVisitor::VisitInitList( InitList* initList )
{
    std::shared_ptr<Type> elemType;

    if ( initList->Values.size() > DataSizeMax )
        mRep.ThrowSemanticsError( initList, "Array initializer is too long" );

    for ( auto& value : initList->Values )
    {
        Visit( value );

        if ( !elemType )
            elemType = value->Type;
        else
            CheckType( elemType, value->Type, value.get() );
    }

    if ( !elemType )
        elemType = mIntType;

    DataSize size = CheckArraySize( initList->Values.size(), elemType.get(), initList );

    initList->Type = Make<ArrayType>( size, elemType );
}

void BinderVisitor::VisitLambdaExpr( LambdaExpr* lambdaExpr )
{
    // In order to limit the special processing and call stack depth,
    // defer the lambda until the the end where it can be treated as
    // a top level procedure

    char name[32];

    snprintf( name, sizeof name, "$Lambda$%zu", mTotalLambdas );

    auto funcType = MakeFuncType( lambdaExpr->Proc.get() );

    auto pointerType = Make<PointerType>( funcType );

    // Set the name up front, so the proc can be passed to AddFunc

    lambdaExpr->Proc->Name = name;

    std::shared_ptr<Function> func = AddFunc( lambdaExpr->Proc.get(), false );

    func->Type = funcType;
    func->IsLambda = true;

    lambdaExpr->Proc->Decl = func;

    mLambdas.push_back( std::move( lambdaExpr->Proc ) );
    mTotalLambdas++;

    Unique<NameExpr> nameExpr( new NameExpr() );
    nameExpr->String = name;
    nameExpr->Decl = func;
    nameExpr->Type = funcType;

    Unique<AddrOfExpr> addrOf( new AddrOfExpr() );
    addrOf->Inner = std::move( nameExpr );
    addrOf->Type = pointerType;
    CopyBaseSyntax( *addrOf, *lambdaExpr );

    mReplacementNode = std::move( addrOf );
}

void BinderVisitor::VisitLetStatement( LetStatement* letStmt )
{
    LocalScope localScope( *this );

    for ( auto& binding : letStmt->Variables )
    {
        if ( binding->Kind == SyntaxKind::VarDecl )
        {
            VisitLetBinding( binding.get() );
        }
        else if ( binding->Kind == SyntaxKind::ConstDecl )
        {
            VisitConstBinding( (ConstDecl*) binding.get(), ScopeKind::Local );
        }
    }

    letStmt->Body.Accept( this );

    CheckStatementType( &letStmt->Body );

    letStmt->Type = letStmt->Body.Type;
}

void BinderVisitor::VisitLetBinding( DataDecl* varDecl )
{
    VisitStorage( varDecl, DeclKind::Local );
}

void BinderVisitor::VisitStorage( DataDecl* varDecl, DeclKind declKind )
{
    std::shared_ptr<Type> type;

    if ( varDecl->TypeRef == nullptr )
    {
        if ( varDecl->Initializer != nullptr )
            Visit( varDecl->Initializer );

        if ( varDecl->Initializer == nullptr )
            type = mIntType;
        else
            type = varDecl->Initializer->Type;
    }
    else
    {
        varDecl->TypeRef->Accept( this );

        type = varDecl->TypeRef->ReferentType;

        if ( varDecl->Initializer != nullptr )
            CheckInitializer( type, varDecl->Initializer );
        else
            CheckAllDescendantsHaveDefault( type.get(), varDecl );
    }

    if ( !IsStorageType( type->GetKind() ) )
        mRep.ThrowSemanticsError( varDecl, "Variables cannot take this type" );

    DataSize size = type->GetSize();

    if ( size == 0 )
        THROW_INTERNAL_ERROR( "Bad type" );

    varDecl->Decl = AddStorage( varDecl, type, size, declKind );
    varDecl->Type = type;
}

void BinderVisitor::CheckInitializer(
    const std::shared_ptr<Type>& type,
    const Unique<Syntax>& initializer )
{
    if ( initializer->Kind == SyntaxKind::ArrayInitializer )
    {
        if ( type->GetKind() != TypeKind::Array )
            mRep.ThrowSemanticsError( initializer.get(), "Array initializer is invalid here" );

        auto& arrayInit = (InitList&) *initializer;
        auto& arrayType = (ArrayType&) *type;

        std::shared_ptr<Type> elemType = arrayType.ElemType;

        if ( (size_t) arrayType.Count < arrayInit.Values.size() )
        {
            mRep.ThrowSemanticsError( initializer.get(), "Wrong number of array elements" );
        }

        for ( auto& value : arrayInit.Values )
        {
            CheckInitializer( elemType, value );
        }

        if ( arrayInit.Fill == ArrayFill::Repeat )
        {
            if ( arrayInit.Values.size() < 1 )
                mRep.ThrowSemanticsError( &arrayInit, "Element repetition requires at least one element" );
        }
        else if ( arrayInit.Fill == ArrayFill::Extrapolate )
        {
            if ( arrayInit.Values.size() < 2 )
                mRep.ThrowSemanticsError( &arrayInit, "Element extrapolation requires at least two elements" );

            if ( elemType->GetKind() != TypeKind::Int )
                mRep.ThrowSemanticsError( &arrayInit, "Elements must be integers to extrapolate them" );
        }
        else
        {
            if ( arrayInit.Values.size() < (size_t) arrayType.Count )
                CheckAllDescendantsHaveDefault( elemType.get(), initializer.get() );
        }

        initializer->Type = type;
    }
    else if ( initializer->Kind == SyntaxKind::RecordInitializer )
    {
        if ( type->GetKind() != TypeKind::Record )
            mRep.ThrowSemanticsError( initializer.get(), "Record initializer is invalid here" );

        auto& recordInit = (RecordInitializer&) *initializer;
        auto& recordType = (RecordType&) *type;

        std::set<std::string> alreadyInit;
        SymTable notInit = recordType.Fields;

        for ( auto& fieldInit : recordInit.Fields )
        {
            if ( alreadyInit.find( fieldInit->Name ) != alreadyInit.end() )
                mRep.ThrowSemanticsError( fieldInit.get(), "Field already initialized" );

            auto it = notInit.find( fieldInit->Name );

            if ( it == notInit.end() )
                mRep.ThrowSemanticsError( fieldInit.get(), "Field not found: %s", fieldInit->Name.c_str() );

            auto fieldDecl = it->second;

            CheckInitializer( fieldDecl->GetType(), fieldInit->Initializer );

            fieldInit->Decl = fieldDecl;

            alreadyInit.insert( fieldInit->Name );
            notInit.erase( it );
        }

        for ( auto& [_, decl] : notInit )
        {
            CheckAllDescendantsHaveDefault( decl->GetType().get(), initializer.get() );
        }

        initializer->Type = type;
    }
    else
    {
        initializer->Accept( this );

        CheckType( type, initializer->Type, initializer.get() );
    }
}

void BinderVisitor::CheckAllDescendantsHaveDefault( Type* type, Syntax* node )
{
    if ( type->GetKind() == TypeKind::Array )
    {
        auto arrayType = (ArrayType*) type;

        CheckAllDescendantsHaveDefault( arrayType->ElemType.get(), node );
    }
    else if ( type->GetKind() == TypeKind::Record )
    {
        auto recordType = (RecordType*) type;

        for ( auto& [_, decl] : recordType->Fields )
        {
            CheckAllDescendantsHaveDefault( decl->GetType().get(), node );
        }
    }
    else if ( type->GetKind() == TypeKind::Pointer )
    {
        mRep.ThrowSemanticsError( node, "Pointers must be initialized" );
    }
}

DataSize BinderVisitor::CheckArraySize( size_t rawSize, Type* elemType, Syntax* node )
{
    if ( rawSize > DataSizeMax )
        mRep.ThrowSemanticsError( node, "Size is too big" );

    DataSize size = static_cast<DataSize>(rawSize);

    auto fullSize = static_cast<uint_fast64_t>(size) * elemType->GetSize();

    if ( fullSize > DataSizeMax )
        mRep.ThrowSemanticsError( node, "Size is too big" );

    return size;
}

void BinderVisitor::VisitLoopStatement( LoopStatement* loopStmt )
{
    loopStmt->Body.Accept( this );

    if ( loopStmt->Condition != nullptr )
    {
        Visit( loopStmt->Condition );

        if ( !IsBoolean( loopStmt->Condition->Type->GetKind() ) )
            mRep.ThrowSemanticsError( loopStmt->Condition.get(), "Loop condition only supports integers" );
    }

    loopStmt->Type = mIntType;
}

void BinderVisitor::VisitNameExpr( NameExpr* nameExpr )
{
    auto decl = FindSymbol( nameExpr->String );

    if ( decl != nullptr )
    {
        if ( decl->Kind == DeclKind::Undefined )
            decl = DefineNode( nameExpr->String, (UndefinedDeclaration*) decl.get() );

        nameExpr->Decl = decl;
    }
    else
    {
        mRep.ThrowSemanticsError( nameExpr, "symbol not found '%s'", nameExpr->String.c_str() );
    }

    nameExpr->Type = nameExpr->Decl->GetType();
}

void BinderVisitor::VisitNameTypeRef( NameTypeRef* nameTypeRef )
{
    Visit( nameTypeRef->QualifiedName );

    auto decl = nameTypeRef->QualifiedName->GetDecl();

    if ( decl->Kind != DeclKind::Type )
        mRep.ThrowSemanticsError( nameTypeRef, "Expected a type name" );

    nameTypeRef->Type = mTypeType;
    nameTypeRef->ReferentType = ((TypeDeclaration*) decl)->ReferentType;
}

void BinderVisitor::VisitNativeDecl( NativeDecl* nativeDecl )
{
    if ( nativeDecl->Decl )
        return;

    PrepareToDefine( nativeDecl );

    if ( nativeDecl->OptionalId )
    {
        Visit( nativeDecl->OptionalId );

        if ( nativeDecl->OptionalId->Kind == SyntaxKind::Number )
        {
            assert( ((NumberExpr&) *nativeDecl->OptionalId).Value <= INT32_MAX );
            mPrevNativeId = static_cast<int32_t>( ((NumberExpr&) *nativeDecl->OptionalId).Value );
        }
        else if ( nativeDecl->OptionalId->Kind == SyntaxKind::Name )
        {
            auto decl = nativeDecl->OptionalId->GetDecl();

            if ( decl->Kind != DeclKind::NativeFunc )
                mRep.ThrowSemanticsError( nativeDecl->OptionalId.get(), "Name of native expected" );

            mPrevNativeId = ((NativeFunction*) decl)->Id;
        }
        else
        {
            THROW_INTERNAL_ERROR( "" );
        }
    }
    else
    {
        if ( mPrevNativeId == INT32_MAX )
            mRep.ThrowSemanticsError( nativeDecl, "ID of native is out of range" );

        mPrevNativeId++;
    }

    std::shared_ptr<NativeFunction> native( new NativeFunction() );

    native->Type = MakeFuncType( nativeDecl );
    native->Id = mPrevNativeId;

    auto typeIt = mNativeTypeMap.find( native->Id );

    if ( typeIt == mNativeTypeMap.end() )
    {
        mNativeTypeMap.insert( NatTypeMap::value_type( native->Id, native->Type ) );
    }
    else
    {
        if ( !typeIt->second->IsEqual( native->Type.get() ) )
            mRep.ThrowSemanticsError( nativeDecl, "Native ID is assigned a different type: %" PRId32, native->Id );
    }

    mGlobalTable.insert( SymTable::value_type( nativeDecl->Name, native ) );

    nativeDecl->Decl = native;
}

void BinderVisitor::VisitNextStatement( NextStatement* nextStmt )
{
    nextStmt->Type = mXferType;
}

void BinderVisitor::VisitNumberExpr( NumberExpr* numberExpr )
{
    numberExpr->Type = mIntType;
}

void BinderVisitor::VisitParamDecl( ParamDecl* paramDecl )
{
    auto type = VisitParamTypeRef( paramDecl->TypeRef, paramDecl->Mode );

    ParamSize size = GetParamSize( type.get(), paramDecl->Mode );

    paramDecl->Decl = AddParam( paramDecl, type, paramDecl->Mode, size );
}

std::shared_ptr<Type> BinderVisitor::VisitParamTypeRef( Unique<TypeRef>& typeRef, ParamMode mode )
{
    std::shared_ptr<Type> type;

    if ( typeRef )
    {
        typeRef->Accept( this );

        if ( !IsAllowedParamType( typeRef->ReferentType->GetKind(), mode ) )
            mRep.ThrowSemanticsError( typeRef.get(), "This type is not allowed for parameters" );

        type = typeRef->ReferentType;
    }
    else
    {
        type = mIntType;
    }

    return type;
}

void BinderVisitor::VisitPointerTypeRef( PointerTypeRef* pointerTypeRef )
{
    pointerTypeRef->Target->Accept( this );

    if ( !IsAllowedPointerTarget( pointerTypeRef->Target->ReferentType->GetKind() ) )
        mRep.ThrowSemanticsError( pointerTypeRef->Target.get(), "This type is not allowed for pointers" );

    auto pointerType = Make<PointerType>( pointerTypeRef->Target->ReferentType );

    pointerTypeRef->Type = mTypeType;
    pointerTypeRef->ReferentType = pointerType;
}

void BinderVisitor::VisitProcDecl( ProcDecl* procDecl )
{
    if ( procDecl->Decl )
        return;

    PrepareToDefine( procDecl );

    auto func = AddFunc( procDecl, true );

    func->Type = MakeFuncType( procDecl );

    procDecl->Decl = func;
}

void BinderVisitor::BindNamedProc( ProcDecl* procDecl )
{
    SymTable::iterator it = mGlobalTable.find( procDecl->Name );
    std::shared_ptr<Function> func;

    if ( it != mGlobalTable.end() )
    {
        if ( it->second->Kind != DeclKind::Func )
        {
            mRep.ThrowSemanticsError( procDecl, "The symbol '%s' is not a function", procDecl->Name.c_str() );
        }
    }
    else
    {
        THROW_INTERNAL_ERROR( "Function wasn't previously declared" );
    }

    VisitProc( procDecl );
}

void BinderVisitor::VisitProc( ProcDecl* procDecl )
{
    LocalScope paramScope( *this );

    auto func = (Function*) procDecl->Decl.get();

    if ( procDecl->Params.size() > ProcDecl::MaxParams )
    {
        mRep.ThrowSemanticsError( procDecl, "'%s' has too many parameters. Max is %d",
            procDecl->Name.c_str(), ProcDecl::MaxParams );
    }

    mParamCount = 0;

    for ( auto& parameter : procDecl->Params )
    {
        parameter->Accept( this );
    }

    mMaxLocalCount = 0;
    mCurLocalCount = 0;

    mCurFunc = func;

    procDecl->Body.Accept( this );

    mCurFunc = nullptr;

    // As locals are added and bound, we check that max count is not exceeded.
    // So there's no need to check it here.

    func->LocalCount = mMaxLocalCount;
    func->ParamCount = mParamCount;

    auto funcType = (FuncType*) func->Type.get();

    CheckType( funcType->ReturnType, procDecl->Body.Type, &procDecl->Body );
}

void BinderVisitor::VisitProcTypeRef( ProcTypeRef* procTypeRef )
{
    std::shared_ptr<Type> returnType = VisitFuncReturnType( procTypeRef->ReturnTypeRef );

    auto funcType = Make<FuncType>( returnType );

    for ( auto& param : procTypeRef->Params )
    {
        param.TypeRef->Accept( this );

        ParamSpec paramSpec;
        paramSpec.Mode = param.Mode;
        paramSpec.Type = param.TypeRef->ReferentType;
        paramSpec.Size = GetParamSize( paramSpec.Type.get(), param.Mode );

        funcType->Params.push_back( paramSpec );
    }

    procTypeRef->Type = mTypeType;
    procTypeRef->ReferentType = funcType;
}

void BinderVisitor::VisitRecordTypeRef( RecordTypeRef* recordTypeRef )
{
    auto recordType = Make<RecordType>();

    DataSize offset = 0;

    for ( auto& fieldDef : recordTypeRef->Fields )
    {
        CheckDuplicateSymbol( fieldDef.get(), recordType->Fields );

        fieldDef->Accept( this );

        if ( fieldDef->Type->GetSize() > (DataSizeMax - offset) )
            mRep.ThrowSemanticsError( fieldDef.get(), "Record type is too big" );

        auto field = Make<FieldStorage>();

        field->Offset = offset;
        field->Type = fieldDef->Type;

        offset += field->Type->GetSize();

        recordType->Fields.insert( SymTable::value_type( fieldDef->Name, field ) );
    }

    recordTypeRef->Type = mTypeType;
    recordTypeRef->ReferentType = recordType;
}

void BinderVisitor::VisitReturnStatement( ReturnStatement* retStmt )
{
    Visit( retStmt->Inner );

    auto funcType = (FuncType*) mCurFunc->Type.get();

    CheckType( funcType->ReturnType, retStmt->Inner->Type, retStmt );

    retStmt->Type = mXferType;
}

void BinderVisitor::VisitSliceExpr( SliceExpr* sliceExpr )
{
    Visit( sliceExpr->Head );
    Visit( sliceExpr->FirstIndex );
    Visit( sliceExpr->LastIndex );

    if ( sliceExpr->Head->Type->GetKind() != TypeKind::Array )
        mRep.ThrowSemanticsError( sliceExpr->Head.get(), "Only arrays can be sliced" );

    if ( sliceExpr->FirstIndex->Type->GetKind() != TypeKind::Int )
        mRep.ThrowSemanticsError( sliceExpr->FirstIndex.get(), "Range bounds must be integers" );

    if ( sliceExpr->LastIndex->Type->GetKind() != TypeKind::Int )
        mRep.ThrowSemanticsError( sliceExpr->LastIndex.get(), "Range bounds must be integers" );

    auto arrayType = (ArrayType*) sliceExpr->Head->Type.get();

    int32_t firstVal = Evaluate( sliceExpr->FirstIndex.get() );
    int32_t lastVal = Evaluate( sliceExpr->LastIndex.get() );

    if ( firstVal >= lastVal )
        mRep.ThrowSemanticsError( sliceExpr->LastIndex.get(), "Range is not in increasing order" );

    if ( firstVal < 0 )
        mRep.ThrowSemanticsError( sliceExpr->LastIndex.get(), "Slices must be within bounds of array" );

    if ( arrayType->Count > 0 )
    {
        if ( lastVal > arrayType->Count )
            mRep.ThrowSemanticsError( sliceExpr->LastIndex.get(), "Slices must be within bounds of array" );
    }

    int32_t rawSize = lastVal - firstVal;

    DataSize size = CheckArraySize( static_cast<size_t>(rawSize), arrayType->ElemType.get(), sliceExpr );

    auto slicedArrayType = Make<ArrayType>( size, arrayType->ElemType );

    sliceExpr->Type = slicedArrayType;
}

void BinderVisitor::VisitStatementList( StatementList* stmtList )
{
    for ( auto& stmt : stmtList->Statements )
    {
        Visit( stmt );
    }

    if ( stmtList->Statements.size() == 0 )
    {
        stmtList->Type = mIntType;
    }
    else
    {
        if ( !IsStatementType( stmtList->Statements.back()->Type->GetKind() ) )
        {
            // REWRITING TREE:
            Unique<NumberExpr> zero( new NumberExpr() );

            zero->Accept( this );

            stmtList->Statements.push_back( std::move( zero ) );
        }

        stmtList->Type = stmtList->Statements.back()->Type;
    }
}

void BinderVisitor::VisitTypeDecl( TypeDecl* typeDecl )
{
    if ( typeDecl->Decl )
        return;

    PrepareToDefine( typeDecl );

    typeDecl->TypeRef->Accept( this );

    typeDecl->Decl = AddType( typeDecl, typeDecl->TypeRef->ReferentType, true );
}

void BinderVisitor::VisitUnaryExpr( UnaryExpr* unary )
{
    Visit( unary->Inner );

    if ( unary->Inner->Type->GetKind() != TypeKind::Int )
        mRep.ThrowSemanticsError( unary->Inner.get(), "Unary expression only supports integers" );

    unary->Type = unary->Inner->Type;
}

void BinderVisitor::VisitUnit( Unit* unit )
{
    for ( auto& varNode : unit->DataDeclarations )
        varNode->Accept( this );

    for ( auto& funcNode : unit->FuncDeclarations )
        funcNode->Accept( this );
}

void BinderVisitor::VisitVarDecl( VarDecl* varDecl )
{
    if ( varDecl->Decl )
        return;

    PrepareToDefine( varDecl );

    VisitStorage( varDecl, DeclKind::Global );
}

void BinderVisitor::VisitWhileStatement( WhileStatement* whileStmt )
{
    Visit( whileStmt->Condition );

    if ( !IsBoolean( whileStmt->Condition->Type->GetKind() ) )
        mRep.ThrowSemanticsError( whileStmt->Condition.get(), "While condition only supports integers" );

    whileStmt->Body.Accept( this );

    whileStmt->Type = mIntType;
}


void BinderVisitor::BindLambdas( Unit* unit )
{
    for ( auto& proc : mLambdas )
    {
        // The proc already has a Declaration object
        VisitProc( proc.get() );

        unit->FuncDeclarations.push_back( std::move( proc ) );
    }

    // Empty the list, so that another unit doesn't try to process them
    mLambdas.clear();
}


void BinderVisitor::CheckType(
    const std::shared_ptr<Type>& site,
    const std::shared_ptr<Type>& type,
    Syntax* node )
{
    CheckType( site.get(), type.get(), node );
}

void BinderVisitor::CheckType(
    Type* site,
    Type* type,
    Syntax* node )
{
    if ( site->GetKind() != TypeKind::Xfer
        && type->GetKind() != TypeKind::Xfer
        && !site->IsAssignableFrom( type ) )
    {
        mRep.ThrowSemanticsError( node, "Incompatible assignment" );
    }
}

void BinderVisitor::CheckStatementType( Syntax* node )
{
    if ( !IsStatementType( node->Type->GetKind() ) )
        mRep.ThrowSemanticsError( node, "Expected scalar type" );
}

void BinderVisitor::CheckArgument(
    ParamMode mode,
    const std::shared_ptr<Type>& site,
    Syntax* argNode )
{
    const auto& type = argNode->Type;

    if ( site->GetKind() != TypeKind::Xfer
        && type->GetKind() != TypeKind::Xfer
        && !site->IsPassableFrom( type.get(), mode ) )
    {
        mRep.ThrowSemanticsError( argNode, "Incompatible argument type" );
    }

    if ( mode == ParamMode::InOutRef )
    {
        auto decl = argNode->GetDecl();

        if ( argNode->Kind != SyntaxKind::Index
            && argNode->Kind != SyntaxKind::Slice
            && (decl == nullptr || !IsVarDeclaration( decl->Kind )) )
            mRep.ThrowSemanticsError( argNode, "Expression can't be passed by reference" );
    }
}

void BinderVisitor::CheckAndConsolidateClauseType( StatementList& clause, std::shared_ptr<Type>& bodyType )
{
    CheckAndConsolidateClauseType( &clause, bodyType );
}

void BinderVisitor::CheckAndConsolidateClauseType( Syntax* clause, std::shared_ptr<Type>& bodyType )
{
    CheckStatementType( clause );

    if ( !bodyType || bodyType->GetKind() == TypeKind::Xfer )
        bodyType = clause->Type;
    else
        CheckType( bodyType, clause->Type, clause );
}

int32_t BinderVisitor::Evaluate( Syntax* node, const char* message )
{
    FolderVisitor folder( mRep.GetLog() );

    auto optValue = folder.Evaluate( node );

    if ( optValue.has_value() )
        return optValue.value();

    if ( message != nullptr )
        mRep.ThrowSemanticsError( node, message );
    else
        mRep.ThrowSemanticsError( node, "Expected a constant value" );
}

std::optional<int32_t> BinderVisitor::GetOptionalSyntaxValue( Syntax* node )
{
    FolderVisitor folder( mRep.GetLog() );

    return folder.Evaluate( node );
}


std::shared_ptr<Declaration> BinderVisitor::FindSymbol( const std::string& symbol )
{
    for ( auto stackIt = mSymStack.rbegin(); stackIt != mSymStack.rend(); stackIt++ )
    {
        auto table = *stackIt;

        auto it = table->find( symbol );
        if ( it != table->end() )
            return it->second;
    }

    return nullptr;
}

std::shared_ptr<ParamStorage> BinderVisitor::AddParam( DeclSyntax* declNode, std::shared_ptr<Type> type, ParamMode mode, size_t size )
{
    auto& table = *mSymStack.back();

    CheckDuplicateSymbol( declNode, table );

    if ( size > static_cast<size_t>(ParamSizeMax - mParamCount) )
        mRep.ThrowSemanticsError( declNode, "Param exceeds capacity" );

    std::shared_ptr<ParamStorage> param( new ParamStorage() );
    param->Offset = mParamCount;
    param->Type = type;
    param->Mode = mode;
    param->Size = static_cast<ParamSize>( size );
    table.insert( SymTable::value_type( declNode->Name, param ) );

    mParamCount += static_cast<ParamSize>( size );
    return param;
}

std::shared_ptr<LocalStorage> BinderVisitor::AddLocal( DeclSyntax* declNode, std::shared_ptr<Type> type, size_t size )
{
    assert( size >= 1 );

    auto& table = *mSymStack.back();

    CheckDuplicateSymbol( declNode, table );

    if ( size > static_cast<size_t>( LocalSizeMax - mCurLocalCount ) )
        mRep.ThrowSemanticsError( declNode, "Local exceeds capacity" );

    std::shared_ptr<LocalStorage> local( new LocalStorage() );
    local->Offset = static_cast<LocalSize>( mCurLocalCount + size - 1 );
    local->Type = type;
    mSymStack.back()->insert( SymTable::value_type( declNode->Name, local ) );

    mCurLocalCount += static_cast<LocalSize>( size );
    mCurLevelLocalCount += static_cast<LocalSize>( size );

    if ( mCurLocalCount > mMaxLocalCount )
        mMaxLocalCount = mCurLocalCount;

    return local;
}

std::shared_ptr<GlobalStorage> BinderVisitor::AddGlobal( DeclSyntax* declNode, std::shared_ptr<Type> type, size_t size )
{
    CheckDuplicateGlobalSymbol( declNode );

    if ( size > static_cast<size_t>( GlobalSizeMax - mGlobalSize ) )
        mRep.ThrowSemanticsError( declNode, "Global exceeds capacity" );

    std::shared_ptr<GlobalStorage> global( new GlobalStorage() );
    global->Offset = mGlobalSize;
    global->ModIndex = mModIndex;
    global->Type = type;
    mGlobalTable.insert( SymTable::value_type( declNode->Name, global ) );

    mGlobalSize += static_cast<GlobalSize>( size );

    mPublicTable.insert( SymTable::value_type( declNode->Name, global ) );

    return global;
}

std::shared_ptr<Declaration> BinderVisitor::AddStorage( DeclSyntax* declNode, std::shared_ptr<Type> type, size_t size, DeclKind declKind )
{
    switch ( declKind )
    {
    case DeclKind::Global:  return AddGlobal( declNode, type, size );
    case DeclKind::Local:   return AddLocal( declNode, type, size );
    default:
        THROW_INTERNAL_ERROR( "" );
    }
}

std::shared_ptr<Constant> BinderVisitor::AddConst( DeclSyntax* declNode, std::shared_ptr<Type> type, int32_t value, SymTable& table )
{
    CheckDuplicateSymbol( declNode, table );

    std::shared_ptr<SimpleConstant> constant( new SimpleConstant() );
    constant->Type = type;
    constant->Value = value;
    table.insert( SymTable::value_type( declNode->Name, constant ) );
    return constant;
}

std::shared_ptr<Constant> BinderVisitor::AddConst( DeclSyntax* declNode, std::shared_ptr<Type> type, int32_t value, bool isPublic )
{
    auto constant = AddConst( declNode, type, value, mGlobalTable );

    if ( isPublic )
        mPublicTable.insert( SymTable::value_type( declNode->Name, constant ) );

    return constant;
}

std::shared_ptr<Function> BinderVisitor::AddFunc( DeclSyntax* declNode, bool isPublic )
{
    CheckDuplicateGlobalSymbol( declNode );

    std::shared_ptr<Function> func( new Function() );
    func->Name = declNode->Name;
    func->Address = UndefinedAddr;
    func->ModIndex = mModIndex;
    mGlobalTable.insert( SymTable::value_type( declNode->Name, func ) );

    if ( isPublic )
        mPublicTable.insert( SymTable::value_type( declNode->Name, func ) );

    return func;
}

std::shared_ptr<TypeDeclaration> BinderVisitor::AddType( DeclSyntax* declNode, std::shared_ptr<Type> type, bool isPublic )
{
    CheckDuplicateGlobalSymbol( declNode );

    std::shared_ptr<TypeDeclaration> typeDecl( new TypeDeclaration() );
    typeDecl->Type = mTypeType;
    typeDecl->ReferentType = type;
    mGlobalTable.insert( SymTable::value_type( declNode->Name, typeDecl ) );

    if ( isPublic )
        mPublicTable.insert( SymTable::value_type( declNode->Name, typeDecl ) );

    return typeDecl;
}

void BinderVisitor::AddModule( DeclSyntax* declNode, std::shared_ptr<ModuleDeclaration> moduleDecl )
{
    CheckDuplicateGlobalSymbol( declNode );

    mGlobalTable.insert( SymTable::value_type( declNode->Name, moduleDecl ) );
}

void BinderVisitor::CheckDuplicateGlobalSymbol( DeclSyntax* declNode )
{
    CheckDuplicateSymbol( declNode, mGlobalTable );
}

void BinderVisitor::CheckDuplicateSymbol( DeclSyntax* declNode, const SymTable& table )
{
    if ( table.find( declNode->Name ) != table.end() )
        mRep.ThrowSemanticsError( declNode, "Duplicate symbol: %s", declNode->Name.c_str() );
}

void BinderVisitor::MakeStdEnv()
{
    mErrorType.reset( new ErrorType() );
    mTypeType.reset( new TypeType() );
    mModuleType.reset( new ModuleType() );
    mXferType.reset( new XferType() );
    mIntType.reset( new IntType() );

    // Dummy syntax nodes to conform to the Add-declaration API's

    VarDecl intSyntax( "int" );
    VarDecl falseSyntax( "false" );
    VarDecl trueSyntax( "true" );

    AddType( &intSyntax, mIntType, false );
    AddConst( &falseSyntax, mIntType, 0, false );
    AddConst( &trueSyntax, mIntType, 1, false );
}

void BinderVisitor::BindProcs( Unit* program )
{
    for ( auto& elem : program->FuncDeclarations )
    {
        BindNamedProc( elem.get() );
    }
}

void BinderVisitor::DeclareNode( DeclSyntax* node )
{
    CheckDuplicateGlobalSymbol( node );

    std::shared_ptr<UndefinedDeclaration> undef( new UndefinedDeclaration() );
    undef->Node = node;
    undef->Type = mErrorType;
    mGlobalTable.insert( SymTable::value_type( node->Name, undef ) );
}

void BinderVisitor::PrepareToDefine( DeclSyntax* declNode )
{
    // The easiest way to fail if there are circular references is to delete the
    // UndefinedDeclaration. But the error you get from a circular reference is
    // only acceptable.

    // For a little better diagnostic, you'd have to keep the UndefinedDeclaration,
    // and check whether the definition is in progress.

    mGlobalTable.erase( declNode->Name );
}

std::shared_ptr<Declaration> BinderVisitor::DefineNode( const std::string& name, UndefinedDeclaration* decl )
{
    // The first thing that the declaration nodes do is to erase the "undefined" declaration
    // to make room for the defined declaration. This also prevents getting stuck in loops.
    // Save the node that the undefined declaration refers to.

    Syntax* node = decl->Node;

    node->Accept( this );

    return FindSymbol( name );
}

std::shared_ptr<FuncType> BinderVisitor::MakeFuncType( ProcDeclBase* procDecl )
{
    std::shared_ptr<Type> returnType = VisitFuncReturnType( procDecl->ReturnTypeRef );

    auto funcType = Make<FuncType>( returnType );

    for ( auto& paramDataDecl : procDecl->Params )
    {
        auto paramDecl = (ParamDecl*) paramDataDecl.get();

        auto type = VisitParamTypeRef( paramDecl->TypeRef, paramDecl->Mode );

        ParamSpec paramSpec;
        paramSpec.Mode = paramDecl->Mode;
        paramSpec.Type = type;
        paramSpec.Size = GetParamSize( type.get(), paramDecl->Mode );

        funcType->Params.push_back( paramSpec );
    }

    return funcType;
}

std::shared_ptr<Type> BinderVisitor::VisitFuncReturnType( Unique<TypeRef>& typeRef )
{
    if ( typeRef )
    {
        typeRef->Accept( this );

        if ( !IsScalarType( typeRef->ReferentType->GetKind() ) )
            mRep.ThrowSemanticsError( typeRef.get(), "Only scalar types can be returned" );

        return typeRef->ReferentType;
    }

    return mIntType;
}

void BinderVisitor::Visit( Unique<Syntax>& child )
{
    child->Accept( this );

    if ( mReplacementNode )
    {
        child = std::move( mReplacementNode );
    }
}

ParamSize BinderVisitor::GetParamSize( Type* type, ParamMode mode )
{
    switch ( mode )
    {
    case ParamMode::Value:
        {
            auto size = type->GetSize();
            if ( size > ParamSizeMax )
                mRep.ThrowSemanticsError( NULL, "Parameter is too big" );

            return static_cast<ParamSize>(size);
        }

    case ParamMode::InOutRef:
        // Open array: dope vector + address
        if ( type->GetKind() == TypeKind::Array && ((ArrayType&) *type).Count == 0 )
            return 2;

        // Closed array: address
        return 1;

    default:
        assert( false );
        return 1;
    }
}

}

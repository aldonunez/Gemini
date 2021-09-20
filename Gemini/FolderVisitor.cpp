// Gemini Languages and Virtual Machine
// Copyright 2021 Aldo Jose Nunez
//
// Licensed under the Apache License, Version 2.0.
// See the LICENSE.txt file for details.

#include "pch.h"
#include "FolderVisitor.h"
#include "VmCommon.h"
#include <assert.h>


namespace Gemini
{

FolderVisitor::FolderVisitor( ICompilerLog* log ) :
    mFoldNodes( false ),
    mRep( log )
{
}

std::optional<int32_t> FolderVisitor::Evaluate( Syntax* node )
{
    mFoldNodes = false;
    node->Accept( this );

    return mLastValue;
}

void FolderVisitor::Fold( Syntax* node, ConstIndexFuncMap& constIndexFuncMap )
{
    mFoldNodes = true;
    mConstIndexFuncMap = &constIndexFuncMap;
    node->Accept( this );
}

void FolderVisitor::VisitAddrOfExpr( AddrOfExpr* addrOf )
{
    mLastValue.reset();
}

void FolderVisitor::VisitArrayTypeRef( ArrayTypeRef* typeRef )
{
    mLastValue.reset();
}

void FolderVisitor::VisitAsExpr( AsExpr* asExpr )
{
    Fold( asExpr->Inner );
}

void FolderVisitor::VisitAssignmentExpr( AssignmentExpr* assignment )
{
    assignment->Left->Accept( this );
    Fold( assignment->Right );
    mLastValue.reset();
}

void FolderVisitor::VisitBinaryExpr( BinaryExpr* binary )
{
    Fold( binary->Left );

    std::optional<int32_t> leftOptVal = std::move( mLastValue );

    Fold( binary->Right );

    if ( leftOptVal.has_value() && mLastValue.has_value() )
    {
        int32_t left = leftOptVal.value();
        int32_t right = mLastValue.value();
        int32_t result = 0;

        if ( binary->Op == "+" )
            result = VmAdd( left, right );
        else if ( binary->Op == "-" )
            result = VmSub( left, right );
        else if ( binary->Op == "*" )
            result = VmMul( left, right );
        else if ( binary->Op == "/" )
        {
            if ( right == 0 )
                mRep.ThrowSemanticsError( binary->Right.get(), "Division by 0" );

            result = VmDiv( left, right );
        }
        else if ( binary->Op == "%" )
        {
            if ( right == 0 )
                mRep.ThrowSemanticsError( binary->Right.get(), "Division by 0" );

            result = VmMod( left, right );
        }
        else if ( binary->Op == "=" )
            result = left == right;
        else if ( binary->Op == "<>" )
            result = left != right;
        else if ( binary->Op == "<" )
            result = left < right;
        else if ( binary->Op == "<=" )
            result = left <= right;
        else if ( binary->Op == ">" )
            result = left > right;
        else if ( binary->Op == ">=" )
            result = left >= right;
        else if ( binary->Op == "and" )
            result = left && right;
        else if ( binary->Op == "or" )
            result = left || right;
        else
            THROW_INTERNAL_ERROR( "" );

        mLastValue = result;
    }
    else
    {
        mLastValue.reset();
    }
}

void FolderVisitor::VisitBreakStatement( BreakStatement* breakStmt )
{
    mLastValue.reset();
}

void FolderVisitor::VisitCallExpr( CallExpr* call )
{
    auto headType = call->Head->Type;

    if ( headType->GetKind() == TypeKind::Pointer )
        headType = ((PointerType&) *headType).TargetType;

    auto& funcType = (FuncType&) *headType;
    auto paramIt = funcType.Params.cbegin();

    for ( auto& arg : call->Arguments )
    {
        if ( paramIt->Mode == ParamMode::Value || paramIt->Mode == ParamMode::ValueIn )
            Fold( arg );
        else
            arg->Accept( this );

        paramIt++;
    }

    Fold( call->Head );

    mLastValue.reset();
}

void FolderVisitor::VisitCallOrSymbolExpr( CallOrSymbolExpr* callOrSymbol )
{
    callOrSymbol->Symbol->Accept( this );
}

void FolderVisitor::VisitCaseExpr( CaseExpr* caseExpr )
{
    Fold( caseExpr->TestKey );

    for ( auto& clause : caseExpr->Clauses )
    {
        for ( auto& key : clause->Keys )
        {
            Fold( key );
        }

        clause->Body.Accept( this );
    }

    if ( caseExpr->Fallback != nullptr )
        caseExpr->Fallback->Body.Accept( this );

    mLastValue.reset();
}

void FolderVisitor::VisitCondExpr( CondExpr* condExpr )
{
    for ( auto& clause : condExpr->Clauses )
    {
        Fold( clause->Condition );
        clause->Body.Accept( this );
    }

    mLastValue.reset();
}

void FolderVisitor::VisitConstDecl( ConstDecl* constDecl )
{
    // The initializer was already folded by binder

    mLastValue.reset();
}

void FolderVisitor::VisitCountofExpr( CountofExpr* countofExpr )
{
    Fold( countofExpr->Expr );

    auto& arrayType = (ArrayType&) *countofExpr->Expr->Type;

    if ( arrayType.Count != 0 )
    {
        mLastValue = arrayType.Count;
    }
    else
    {
        mLastValue.reset();
    }
}

void FolderVisitor::VisitFieldAccess( DotExpr* dotExpr )
{
    if ( mCalcOffset )
    {
        dotExpr->Head->Accept( this );

        if ( mBufOffset.has_value() && mBuffer )
        {
            auto& recordType = (RecordType&) *dotExpr->Head->Type;

            auto fieldIt = recordType.GetFields().find( dotExpr->Member );

            mBufOffset = ((FieldStorage&) *fieldIt->second).Offset + mBufOffset.value();
        }
        else
        {
            mLastValue.reset();
            mBufOffset.reset();
            mBuffer.reset();
        }
    }
    else
    {
        mCalcOffset = true;
        dotExpr->Accept( this );
        mCalcOffset = false;

        if ( mBufOffset.has_value() && mBuffer )
        {
            mLastValue = (*mBuffer)[mBufOffset.value()];
        }
        else
        {
            mLastValue.reset();
            mBufOffset.reset();
            mBuffer.reset();
        }
    }
}

void FolderVisitor::VisitDotExpr( DotExpr* dotExpr )
{
    if ( dotExpr->GetDecl()->Kind == DeclKind::Const
        && Is( ((Constant*) dotExpr->GetDecl())->Value, ValueKind::Integer ) )
    {
        mLastValue = Get<ValueKind::Integer>( ((Constant*) dotExpr->GetDecl())->Value );
    }
    else if ( dotExpr->Head->Type->GetKind() == TypeKind::Record )
    {
        VisitFieldAccess( dotExpr );
    }
    else
    {
        Fold( dotExpr->Head );

        mLastValue.reset();
    }
}

void FolderVisitor::VisitForStatement( ForStatement* forStmt )
{
    Fold( forStmt->First );
    Fold( forStmt->Last );

    if ( forStmt->Step )
        Fold( forStmt->Step );

    forStmt->Body.Accept( this );

    mLastValue.reset();
}

void FolderVisitor::VisitIndexExpr( IndexExpr* indexExpr )
{
    if ( mCalcOffset )
    {
        indexExpr->Head->Accept( this );

        auto bufOffset = mBufOffset;
        auto buffer = mBuffer;

        mCalcOffset = false;
        Fold( indexExpr->Index );
        mCalcOffset = true;

        if ( mLastValue.has_value() && bufOffset.has_value() && buffer )
        {
            mBufOffset = (mLastValue.value() * indexExpr->Type->GetSize()) + mBufOffset.value();
        }
        else
        {
            mLastValue.reset();
            mBufOffset.reset();
            mBuffer.reset();
        }
    }
    else
    {
        mCalcOffset = true;
        indexExpr->Accept( this );
        mCalcOffset = false;

        if ( mBufOffset.has_value() && mBuffer )
        {
            mLastValue = (*mBuffer)[mBufOffset.value()];
        }
        else
        {
            mLastValue.reset();
            mBufOffset.reset();
            mBuffer.reset();
        }
    }
}

void FolderVisitor::VisitInitList( InitList* initList )
{
    for ( auto& value : initList->Values )
    {
        Fold( value );
    }

    mLastValue.reset();
}

void FolderVisitor::VisitLambdaExpr( LambdaExpr* lambdaExpr )
{
    VisitProc( lambdaExpr->Proc.get() );
    mLastValue.reset();
}

void FolderVisitor::VisitLetStatement( LetStatement* letStmt )
{
    for ( auto& binding : letStmt->Variables )
    {
        VisitLetBinding( binding.get() );
    }

    letStmt->Body.Accept( this );
    mLastValue.reset();
}

void FolderVisitor::VisitLetBinding( DataDecl* varDecl )
{
    if ( varDecl->Initializer != nullptr )
        Fold( varDecl->Initializer );
}

void FolderVisitor::VisitLoopStatement( LoopStatement* loopStmt )
{
    loopStmt->Body.Accept( this );

    if ( loopStmt->Condition != nullptr )
        Fold( loopStmt->Condition );

    mLastValue.reset();
}

void FolderVisitor::VisitNameExpr( NameExpr* nameExpr )
{
    mBufOffset.reset();

    if ( nameExpr->Decl->Kind == DeclKind::Const
        && Is( ((Constant*) nameExpr->GetDecl())->Value, ValueKind::Integer ) )
    {
        mLastValue = Get<ValueKind::Integer>( ((Constant*) nameExpr->Decl.get())->Value );
    }
    else if ( mCalcOffset && nameExpr->Decl->Kind == DeclKind::Const
        && Is( ((Constant*) nameExpr->GetDecl())->Value, ValueKind::Aggregate ) )
    {
        mBufOffset = 0;
        mBuffer = Get<ValueKind::Aggregate>( ((Constant*) nameExpr->Decl.get())->Value );
    }
    else
    {
        mLastValue.reset();
        mBuffer.reset();
    }
}

void FolderVisitor::VisitNextStatement( NextStatement* nextStmt )
{
    mLastValue.reset();
}

void FolderVisitor::VisitNumberExpr( NumberExpr* numberExpr )
{
    assert( numberExpr->Value >= INT32_MIN );

    if ( numberExpr->Value > INT32_MAX )
        mRep.ThrowSemanticsError( numberExpr, "Number out of range" );

    mLastValue = (int32_t) numberExpr->Value;
}

void FolderVisitor::VisitParamDecl( ParamDecl* paramDecl )
{
    mLastValue.reset();
}

void FolderVisitor::VisitProcDecl( ProcDecl* procDecl )
{
    VisitProc( procDecl );
    mLastValue.reset();
}

void FolderVisitor::VisitProc( ProcDecl* procDecl )
{
    for ( auto& parameter : procDecl->Params )
    {
        parameter->Accept( this );
    }

    procDecl->Body.Accept( this );
    mLastValue.reset();
}

void FolderVisitor::VisitRecordInitializer( RecordInitializer* recordInitializer )
{
    for ( auto& fieldInit : recordInitializer->Fields )
    {
        Fold( fieldInit->Initializer );
    }

    mLastValue.reset();
}

void FolderVisitor::VisitReturnStatement( ReturnStatement* retStmt )
{
    Fold( retStmt->Inner );
    mLastValue.reset();
}

void FolderVisitor::VisitSliceExpr( SliceExpr* sliceExpr )
{
    sliceExpr->Head->Accept( this );
    Fold( sliceExpr->FirstIndex );
    Fold( sliceExpr->LastIndex );
    mLastValue.reset();
}

void FolderVisitor::VisitStatementList( StatementList* stmtList )
{
    for ( auto& stmt : stmtList->Statements )
    {
        Fold( stmt );
    }

    mLastValue.reset();
}

void FolderVisitor::VisitUnaryExpr( UnaryExpr* unary )
{
    if ( unary->Op == "-" && unary->Inner->Kind == SyntaxKind::Number )
    {
        int64_t value = ((NumberExpr&) *unary->Inner).Value;

        assert( value >= 0 && value <= (uint32_t) INT32_MAX + 1 );

        mLastValue = (int32_t) -value;
        return;
    }

    Fold( unary->Inner );

    if ( mLastValue.has_value() )
    {
        if ( unary->Op == "-" )
            mLastValue = VmSub( 0, mLastValue.value() );
        else if ( unary->Op == "not" )
            mLastValue = !mLastValue.value();
        else
            THROW_INTERNAL_ERROR( "" );
    }
}

void FolderVisitor::VisitUnit( Unit* unit )
{
    for ( auto& varNode : unit->DataDeclarations )
        varNode->Accept( this );

    for ( auto& funcNode : unit->FuncDeclarations )
        funcNode->Accept( this );
}

void FolderVisitor::VisitVarDecl( VarDecl* varDecl )
{
    if ( varDecl->Initializer != nullptr )
        Fold( varDecl->Initializer );

    mLastValue.reset();
}

void FolderVisitor::VisitWhileStatement( WhileStatement* whileStmt )
{
    Fold( whileStmt->Condition );
    whileStmt->Body.Accept( this );
    mLastValue.reset();
}


void FolderVisitor::Fold( Unique<Syntax>& child )
{
    child->Accept( this );

    if ( !mFoldNodes )
        return;

    if ( mLastValue.has_value() && IsIntegralType( child->Type->GetKind() ) )
    {
        if ( child->Kind != SyntaxKind::Number )
        {
            Unique<NumberExpr> number( new NumberExpr( mLastValue.value() ) );

            if ( !mIntType )
                mIntType = std::shared_ptr<IntType>( new IntType() );

            child = std::move( number );
            child->Type = mIntType;
        }
    }
    else if ( mLastValue.has_value() && IsPtrFuncType( *child->Type ) )
    {
        if ( mConstIndexFuncMap == nullptr )
            THROW_INTERNAL_ERROR( "" );

        auto funcIt = mConstIndexFuncMap->find( mLastValue.value() );

        if ( funcIt == mConstIndexFuncMap->end() )
            THROW_INTERNAL_ERROR( "" );

        auto func = funcIt->second;

        auto pointerType = std::shared_ptr<PointerType>( new PointerType( func->Type ) );

        Unique<NameExpr> nameExpr( new NameExpr() );
        nameExpr->String = "<const>";
        nameExpr->Decl = func;
        nameExpr->Type = func->Type;

        Unique<AddrOfExpr> addrOf( new AddrOfExpr() );
        addrOf->Inner = std::move( nameExpr );
        addrOf->Type = pointerType;
        CopyBaseSyntax( *addrOf, *child );

        child = std::move( addrOf );
    }
}


//----------------------------------------------------------------------------
//  FuncAddrVisitor
//----------------------------------------------------------------------------

FuncAddrVisitor::FuncAddrVisitor( ICompilerLog* log ) :
    mRep( log )
{
}

std::shared_ptr<Function> FuncAddrVisitor::Evaluate( Syntax* node )
{
    node->Accept( this );

    return mLastValue;
}

void FuncAddrVisitor::VisitAddrOfExpr( AddrOfExpr* addrOf )
{
    auto decl = addrOf->Inner->GetSharedDecl();

    if ( decl->Kind != DeclKind::Func )
        mRep.ThrowSemanticsError( addrOf, "Expected function" );

    mLastValue = std::static_pointer_cast<Function>(decl);
}

void FuncAddrVisitor::VisitDotExpr( DotExpr* dotExpr )
{
    VisitDotOrNameExpr( dotExpr );
}

void FuncAddrVisitor::VisitNameExpr( NameExpr* nameExpr )
{
    VisitDotOrNameExpr( nameExpr );
}

void FuncAddrVisitor::VisitDotOrNameExpr( Syntax* expr )
{
    if ( expr->GetDecl()->Kind == DeclKind::Const
        && Is( ((Constant*) expr->GetDecl())->Value, ValueKind::Function ) )
    {
        mLastValue = Get<ValueKind::Function>( ((Constant*) expr->GetDecl())->Value );
    }
    else
    {
        mLastValue.reset();
    }
}

}

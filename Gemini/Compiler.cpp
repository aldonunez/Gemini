// Gemini Languages and Virtual Machine
// Copyright 2019 Aldo Jose Nunez
//
// Licensed under the Apache License, Version 2.0.
// See the LICENSE.txt file for details.

#include "pch.h"
#include "Compiler.h"
#include "BinderVisitor.h"
#include "FolderVisitor.h"
#include "OpCodes.h"
#include <stdarg.h>
#include <string.h>
#include <stdexcept>


namespace Gemini
{

Compiler::Compiler( ICompilerEnv* env, ICompilerLog* log, ModSize modIndex ) :
    mEnv( env ),
    mRep( log ),
    mModIndex( modIndex )
{
    if ( env == nullptr )
        throw std::invalid_argument( "env" );

    if ( log == nullptr )
        throw std::invalid_argument( "log" );

    if ( modIndex > ModSizeMax )
        throw std::invalid_argument( "modIndex" );

    mLoadedAddrDecl.reset( new LoadedAddressDeclaration() );
}

void Compiler::AddUnit( Unique<Unit>&& unit )
{
    if ( !unit )
        throw std::invalid_argument( "unit" );

    mUnits.push_back( std::move( unit ) );
}

void Compiler::AddModule( std::shared_ptr<ModuleDeclaration> moduleDecl )
{
    if ( !moduleDecl )
        throw std::invalid_argument( "moduleDecl" );

    auto modTabResult = mModuleTable.insert( SymTable::value_type( moduleDecl->Name, moduleDecl ) );

    if ( !modTabResult.second )
        throw std::invalid_argument( "Module name already used" );

    auto modIdResult = mModulesById.insert( ModIdMap::value_type( moduleDecl->Index, moduleDecl ) );

    if ( !modIdResult.second )
        throw std::invalid_argument( "Module index already used" );
}

CompilerErr Compiler::Compile()
{
    if ( mStatus != CompilerErr::NONE )
        return mStatus;

    try
    {
        BindAttributes();
        FoldConstants();
        GenerateCode();

        CopyDeferredGlobals();

        GenerateSentinel();

        mStatus = CompilerErr::OK;
    }
    catch ( CompilerException& ex )
    {
        mStatus = ex.GetError();
    }

    return mStatus;
}

void Compiler::GetStats( CompilerStats& stats )
{
    if ( mStatus == CompilerErr::OK && !mCalculatedStats )
    {
        mStats.CodeBytesWritten = static_cast<CodeSize>( mCodeBin.size() );

        CalculateStackDepth();

        mCalculatedStats = true;
    }

    stats = mStats;
}

U8* Compiler::GetCode()
{
    return mCodeBin.data();
}

size_t Compiler::GetCodeSize()
{
    return mCodeBin.size();
}

I32* Compiler::GetData()
{
    return &mGlobals.front();
}

size_t Compiler::GetDataSize()
{
    return mGlobals.size();
}

std::shared_ptr<ModuleDeclaration> Compiler::GetMetadata( const char* modName )
{
    if ( modName == nullptr )
        throw std::invalid_argument( "modName" );

    std::shared_ptr<ModuleDeclaration> modDecl( new ModuleDeclaration() );

    modDecl->Name = modName;
    modDecl->Index = mModIndex;
    modDecl->Table = std::move( mPublicTable );
    modDecl->Type = std::shared_ptr<ModuleType>( new ModuleType() );

    return modDecl;
}

void Compiler::BindAttributes()
{
    BinderVisitor binder( mModIndex, mGlobalTable, mModuleTable, mPublicTable, mRep.GetLog() );

    for ( auto& unit : mUnits )
        binder.Declare( unit.get() );

    for ( auto& unit : mUnits )
        binder.Bind( unit.get() );

    mGlobals.resize( binder.GetDataSize() );
}

void Compiler::FoldConstants()
{
    FolderVisitor folder( mRep.GetLog() );

    for ( auto& unit : mUnits )
        folder.Fold( unit.get() );
}

void Compiler::GenerateCode()
{
    for ( auto& unit : mUnits )
        unit->Accept( this );
}

void Compiler::VisitUnit( Unit* unit )
{
    for ( auto& varNode : unit->DataDeclarations )
        Generate( varNode.get() );

    for ( auto& funcNode : unit->FuncDeclarations )
        funcNode->Accept( this );
}

void Compiler::Generate( Syntax* elem )
{
    GenStatus status = { ExprKind::Other };
    Generate( elem, GenConfig::Statement(), status );
}

void Compiler::Generate( Syntax* elem, const GenConfig& config )
{
    GenStatus status = { ExprKind::Other };
    Generate( elem, config, status );
}

void Compiler::Generate( Syntax* node, const GenConfig& config, GenStatus& status )
{
    mGenStack.push_back( { config, status } );

    node->Accept( this );

    mGenStack.pop_back();

    if ( status.kind != ExprKind::Logical )
    {
        if ( config.trueChain != nullptr )
        {
            OpCode opcode = (config.invert && status.kind != ExprKind::Comparison) ? OP_BFALSE : OP_BTRUE;
            EmitBranch( opcode, config.trueChain );
            DecreaseExprDepth();

            EmitBranch( OP_B, config.falseChain );
        }
        else if ( config.invert && status.kind != ExprKind::Comparison )
        {
            if ( !config.discard )
            {
                Emit( OP_NOT );
            }
        }
    }
}

void Compiler::GenerateDiscard( Syntax* elem )
{
    GenConfig config{};

    GenerateDiscard( elem, config );
}

void Compiler::GenerateDiscard( Syntax* elem, const GenConfig& config )
{
    GenConfig configDiscard = config.WithDiscard();
    GenStatus status = { ExprKind::Other };

    mGenStack.push_back( { configDiscard, status } );

    elem->Accept( this );

    mGenStack.pop_back();

    if ( !status.discarded )
    {
        assert( status.discarded );
        mRep.LogWarning( elem->FileName, elem->Line, elem->Column, "Deprecated: POP was emitted." );

        Emit( OP_POP );

        DecreaseExprDepth();
    }
}

void Compiler::VisitNumberExpr( NumberExpr* numberExpr )
{
    GenerateNumber( numberExpr, Config(), Status() );
}

void Compiler::GenerateNumber( NumberExpr* number, const GenConfig& config, GenStatus& status )
{
    if ( config.discard )
    {
        status.discarded = true;
        return;
    }

    EmitLoadConstant( (int32_t) number->Value );
}

void Compiler::EmitLoadConstant( int32_t value )
{
    if ( (value >= INT8_MIN) && (value <= INT8_MAX) )
    {
        EmitU8( OP_LDC_S, (U8) value );
    }
    else
    {
        EmitU32( OP_LDC, value );
    }

    IncreaseExprDepth();
}

void Compiler::VisitNameExpr( NameExpr* nameExpr )
{
    GenerateSymbol( nameExpr, Config(), Status() );
}

void Compiler::GenerateSymbol( NameExpr* symbol, const GenConfig& config, GenStatus& status )
{
    GenerateValue( symbol, symbol->GetDecl(), config, status );
}

void Compiler::GenerateValue( Syntax* node, Declaration* decl, const GenConfig& config, GenStatus& status )
{
    if ( config.discard )
    {
        status.discarded = true;
        return;
    }
    else if ( config.calcAddr )
    {
        status.baseDecl = decl;
        return;
    }

    if ( IsScalarType( node->Type->GetKind() ) )
    {
        EmitLoadScalar( node, decl, 0 );
    }
    else
    {
        EmitLoadAggregateCopySource( node );
    }
}

void Compiler::EmitLoadScalar( Syntax* node, Declaration* decl, int32_t offset )
{
    switch ( decl->Kind )
    {
    case DeclKind::Global:
        assert( offset >= 0 && offset < GlobalSizeMax );
        assert( offset < (GlobalSizeMax - ((GlobalStorage*) decl)->Offset) );

        EmitModAccess(
            OP_LDMOD,
            ((GlobalStorage*) decl)->ModIndex,
            static_cast<uint16_t>(((GlobalStorage*) decl)->Offset + offset) );
        IncreaseExprDepth();
        break;

    case DeclKind::Local:
        assert( offset >= 0 && offset < LocalSizeMax );
        assert( offset <= ((LocalStorage*) decl)->Offset );

        EmitU8( OP_LDLOC, static_cast<uint8_t>(((LocalStorage*) decl)->Offset - offset) );
        IncreaseExprDepth();
        break;

    case DeclKind::Param:
        assert( offset >= 0 && offset < ParamSizeMax );
        assert( offset < (ParamSizeMax - ((ParamStorage*) decl)->Offset) );

        EmitU8( OP_LDARG, static_cast<uint8_t>(((ParamStorage*) decl)->Offset + offset) );
        IncreaseExprDepth();
        break;

    case DeclKind::Func:
        mRep.ThrowSemanticsError( node, "functions don't have values" );
        break;

    case DeclKind::Const:
        assert( offset == 0 );
        EmitLoadConstant( ((Constant*) decl)->Value );
        break;

    case DeclKind::LoadedAddress:
        assert( offset >= 0 && offset < DataSizeMax );

        if ( offset > 0 )
            EmitSpilledAddrOffset( offset );

        Emit( OP_LOADI );
        DecreaseExprDepth();
        IncreaseExprDepth();
        break;

    default:
        THROW_INTERNAL_ERROR( "" );
    }
}

void Compiler::EmitSpilledAddrOffset( int32_t offset )
{
    // TODO: Consider adding an OFFSET instruction that safely adds a constant offset to an address

    EmitLoadConstant( offset );
    EmitU8( OP_INDEX_S, 1 );

    DecreaseExprDepth();
}

void Compiler::EmitLoadAggregateCopySource( Syntax* node )
{
    EmitLoadAggregateCopySource( node, node->Type.get() );
}

void Compiler::EmitLoadAggregateCopySource( Syntax* node, Type* type )
{
    int32_t         offset = 0;
    Declaration*    decl = nullptr;

    EmitLoadConstant( type->GetSize() );

    CalcAddress( node, decl, offset );

    EmitLoadAddress( node, decl, offset );
}

void Compiler::GenerateEvalStar( CallOrSymbolExpr* callOrSymbol, const GenConfig& config, GenStatus& status )
{
    auto& symbol = callOrSymbol->Symbol;
    auto decl = symbol->GetDecl();

    if ( decl->Kind == DeclKind::Func
        || decl->Kind == DeclKind::NativeFunc )
    {
        std::vector<Unique<Syntax>> args;

        GenerateCall( decl, args, config, status );
    }
    else
    {
        Generate( symbol.get() );
    }
}

void Compiler::VisitCallOrSymbolExpr( CallOrSymbolExpr* callOrSymbol )
{
    GenerateEvalStar( callOrSymbol, Config(), Status() );
}

void Compiler::GenerateArithmetic( BinaryExpr* binary, const GenConfig& config, GenStatus& status )
{
    auto&   op = binary->Op;
    U8      primitive;

    if ( op == "+" )
        primitive = PRIM_ADD;
    else if ( op == "-" )
        primitive = PRIM_SUB;
    else if ( op == "*" )
        primitive = PRIM_MUL;
    else if ( op == "/" )
        primitive = PRIM_DIV;
    else if ( op == "%" )
        primitive = PRIM_MOD;
    else
        THROW_INTERNAL_ERROR( "" );

    GenerateBinaryPrimitive( binary, primitive, config, status );
}

void Compiler::VisitBinaryExpr( BinaryExpr* binary )
{
    if ( binary->Op[0] == '='
        || binary->Op[0] == '<'
        || binary->Op[0] == '>' )
    {
        GenerateComparison( binary, Config(), Status() );
    }
    else if ( binary->Op == "and" )
    {
        GenerateAnd( binary, Config(), Status() );
    }
    else if ( binary->Op == "or" )
    {
        GenerateOr( binary, Config(), Status() );
    }
    else
    {
        GenerateArithmetic( binary, Config(), Status() );
    }
}

void Compiler::GenerateReturn( ReturnStatement* retStmt, const GenConfig& config, GenStatus& status )
{
    if ( retStmt->Inner != nullptr )
    {
        Generate( retStmt->Inner.get() );
    }
    else
    {
        // Currently not allowed in the language
        EmitU8( OP_LDC_S, 0 );
        IncreaseExprDepth();
    }

    Emit( OP_RET );
    DecreaseExprDepth();

    status.discarded = true;
    status.tailRet = true;
}

void Compiler::VisitReturnStatement( ReturnStatement* retStmt )
{
    GenerateReturn( retStmt, Config(), Status() );
}

void Compiler::VisitCountofExpr( CountofExpr* countofExpr )
{
    if ( Config().discard )
    {
        Status().discarded = true;
        return;
    }

    auto& arrayType = (ArrayType&) *countofExpr->Expr->Type;

    if ( arrayType.Count != 0 )
    {
        EmitLoadConstant( arrayType.Count );
    }
    else
    {
        THROW_INTERNAL_ERROR( "" );
    }
}

void Compiler::GenerateCond( CondExpr* condExpr, const GenConfig& config, GenStatus& status )
{
    PatchChain  falseChain;
    PatchChain  leaveChain;
    bool        foundCatchAll = false;
    LocalSize   exprDepth = mCurExprDepth;
    int32_t     startLoc = static_cast<int32_t>( mCodeBin.size() );

    GenConfig statementConfig = GenConfig::Statement( config.discard )
        .WithLoop( config.breakChain, config.nextChain );

    // TODO: check all the clauses for tail-return. If they all do, then set status.tailRet.

    for ( int i = 0; i < (int) condExpr->Clauses.size(); i++ )
    {
        // Restore the expression depth, so that it doesn't accumulate
        mCurExprDepth = exprDepth;

        auto clause = condExpr->Clauses[i].get();

        auto optVal = GetFinalOptionalSyntaxValue( clause->Condition.get() );

        bool isConstantTrue = optVal.has_value() && optVal.value() != 0;

        if ( isConstantTrue )
        {
            if ( clause->Body.Statements.size() == 0 && !condExpr->IsIf )
            {
                GenStatus clauseStatus = { ExprKind::Other };
                Generate( clause->Condition.get(), statementConfig, clauseStatus );
            }
            else
            {
                GenStatus clauseStatus = { ExprKind::Other };
                GenerateImplicitProgn( &clause->Body, statementConfig, clauseStatus );
            }
            foundCatchAll = true;
            break;
        }

        if ( clause->Body.Statements.size() == 0 && !condExpr->IsIf )
        {
            Generate( clause->Condition.get() );

            if ( !config.discard )
            {
                Emit( OP_DUP );
                IncreaseExprDepth();
            }

            EmitBranch( OP_BTRUE, &leaveChain );
            DecreaseExprDepth();
        }
        else
        {
            PatchChain  trueChain;

            falseChain = PatchChain();

            Generate( clause->Condition.get(), GenConfig::Expr( &trueChain, &falseChain, false ) );
            ElideTrue( &trueChain, &falseChain );
            Patch( &trueChain );

            // True
            GenStatus clauseStatus = { ExprKind::Other };
            GenerateImplicitProgn( &clause->Body, statementConfig, clauseStatus );

            if ( i < (int) condExpr->Clauses.size() - 1 || !config.discard )
            {
                // Not the last clause; or last one is not catch-all and not discarding.
                // So, we'll emit LDC.S 0 below. And here we emit a jump over it.

                EmitBranch( OP_B, &leaveChain );
            }

            ElideFalse( &trueChain, &falseChain );
            Patch( &falseChain );
        }
    }

    if ( !foundCatchAll )
    {
        // Restore the expression depth, so that it doesn't accumulate
        mCurExprDepth = exprDepth;

        if ( !config.discard )
        {
            EmitU8( OP_LDC_S, 0 );
            IncreaseExprDepth();
        }
    }

    // This is not very important, because it optimizes a case like:
    //   if n then B() else 3 end
    // ... in a discarding context

    if ( (mCodeBin.size() - startLoc) >= BranchInst::Size
        && mCodeBin[mCodeBin.size() - BranchInst::Size] == OP_B
        && leaveChain.First != nullptr
        && leaveChain.First->Ref == ((int32_t) mCodeBin.size() - BranchInst::Size)
        && falseChain.PatchedInstIndex == (int32_t) mCodeBin.size() )
    {
        PopPatch( &leaveChain );
        DeleteCode( BranchInst::Size );
        Patch( &falseChain );
    }

    Patch( &leaveChain );

    if ( config.discard )
        status.discarded = true;
}

void Compiler::VisitCondExpr( CondExpr* condExpr )
{
    GenerateCond( condExpr, Config(), Status() );
}

void Compiler::GenerateSet( AssignmentExpr* assignment, const GenConfig& config, GenStatus& status )
{
    // Value
    Generate( assignment->Right.get() );

    if ( config.discard )
    {
        status.discarded = true;
    }
    else
    {
        Emit( OP_DUP );
        IncreaseExprDepth();
    }

    int32_t         offset = 0;
    Declaration*    decl = nullptr;

    CalcAddress( assignment->Left.get(), decl, offset );

    EmitStoreScalar( assignment->Left.get(), decl, offset );
}

void Compiler::EmitStoreScalar( Syntax* node, Declaration* decl, int32_t offset )
{
    switch ( decl->Kind )
    {
    case DeclKind::Global:
        assert( offset >= 0 && offset < GlobalSizeMax );
        assert( offset < (GlobalSizeMax - ((GlobalStorage*) decl)->Offset) );

        EmitModAccess(
            OP_STMOD,
            ((GlobalStorage*) decl)->ModIndex,
            static_cast<uint16_t>(((GlobalStorage*) decl)->Offset + offset) );
        break;

    case DeclKind::Local:
        assert( offset >= 0 && offset < LocalSizeMax );
        assert( offset <= ((LocalStorage*) decl)->Offset );

        EmitU8( OP_STLOC, static_cast<uint8_t>(((LocalStorage*) decl)->Offset - offset) );
        break;

    case DeclKind::Param:
        assert( offset >= 0 && offset < ParamSizeMax );
        assert( offset < (ParamSizeMax - ((ParamStorage*) decl)->Offset) );

        EmitU8( OP_STARG, static_cast<uint8_t>(((ParamStorage*) decl)->Offset + offset) );
        break;

    case DeclKind::Func:
    case DeclKind::NativeFunc:
        mRep.ThrowSemanticsError( node, "functions can't be assigned a value" );
        break;

    case DeclKind::Const:
        mRep.ThrowSemanticsError( node, "Constants can't be changed" );
        break;

    case DeclKind::LoadedAddress:
        assert( offset >= 0 && offset < DataSizeMax );

        if ( offset > 0 )
            EmitSpilledAddrOffset( offset );

        Emit( OP_STOREI );
        DecreaseExprDepth();
        break;

    default:
        THROW_INTERNAL_ERROR( "" );
    }

    DecreaseExprDepth();
}

void Compiler::GenerateSetAggregate( AssignmentExpr* assignment, const GenConfig& config, GenStatus& status )
{
    // Value
    Generate( assignment->Right.get() );

    if ( config.discard )
    {
        status.discarded = true;
    }
    else
    {
        Emit( OP_OVER );
        Emit( OP_OVER );
        IncreaseExprDepth();
        IncreaseExprDepth();
    }

    Declaration*    baseDecl = nullptr;
    int32_t         offset = 0;

    CalcAddress( assignment->Left.get(), baseDecl, offset );

    EmitLoadAddress( assignment->Left.get(), baseDecl, offset );

    Emit( OP_COPYBLOCK );

    DecreaseExprDepth( 3 );

    // Generating a return value or argument value would need PUSHBLOCK
}

void Compiler::VisitAssignmentExpr( AssignmentExpr* assignment )
{
    // TODO: Rename GenerateSet GenerateSetScalar

    if ( IsScalarType( assignment->Left->Type->GetKind() ) )
        GenerateSet( assignment, Config(), Status() );
    else
        GenerateSetAggregate( assignment, Config(), Status() );
}

void Compiler::VisitProcDecl( ProcDecl* procDecl )
{
    U32 addr = static_cast<CodeSize>( mCodeBin.size() );

    auto func = (Function*) procDecl->Decl.get();

    func->Address = addr;

    auto patchIt = mFuncPatchMap.find( func->Name );
    if ( patchIt != mFuncPatchMap.end() )
        PatchCalls( &patchIt->second, addr );

    mEnv->AddExternal( procDecl->Name, ExternalKind::Bytecode, func->Address );

    GenerateProc( procDecl, func );
}

void Compiler::VisitLambdaExpr( LambdaExpr* lambdaExpr )
{
    THROW_INTERNAL_ERROR( "LambdaExpr was not transformed" );
}

void Compiler::GenerateFunction( AddrOfExpr* addrOf, const GenConfig& config, GenStatus& status )
{
    if ( config.discard )
    {
        status.discarded = true;
        return;
    }

    auto func = (Function*) addrOf->Inner->GetDecl();

    EmitLoadFuncAddress( func );
}

void Compiler::EmitLoadFuncAddress( Function* func )
{
    size_t curIndex = ReserveCode( 5 );
    mCodeBin[curIndex] = OP_LDC;

    EmitFuncAddress( func, { CodeRefKind::Code, static_cast<int32_t>( curIndex + 1 ) } );

    IncreaseExprDepth();
}

void Compiler::EmitFuncAddress( Function* func, CodeRef funcRef )
{
    if ( func->Address == UndefinedAddr )
    {
        PushFuncPatch( func->Name, funcRef );
    }

    U32 addrWord = CodeAddr::Build( func->Address, func->ModIndex );

    if ( funcRef.Kind == CodeRefKind::Code )
    {
        StoreU32( &mCodeBin[funcRef.Location], addrWord );
    }
    else if ( funcRef.Kind == CodeRefKind::Data )
    {
        mGlobals[funcRef.Location] = addrWord;
    }
}

void Compiler::VisitAddrOfExpr( AddrOfExpr* addrOf )
{
    GenerateFunction( addrOf, Config(), Status() );
}

void Compiler::GenerateFuncall( CallExpr* call, const GenConfig& config, GenStatus& status )
{
    GenerateCallArgs( call->Arguments );

    Generate( call->Head.get() );

    ParamSize argCount = static_cast<ParamSize>( call->Arguments.size() );

    EmitU8( OP_CALLI, CallFlags::Build( argCount, config.discard ) );

    DecreaseExprDepth();

    if ( config.discard )
        status.discarded = true;
    else
        IncreaseExprDepth();

    DecreaseExprDepth( argCount );

    if ( mInFunc )
        mCurFunc->CallsIndirectly = true;
}

void Compiler::GenerateLet( LetStatement* letStmt, const GenConfig& config, GenStatus& status )
{
    for ( auto& binding : letStmt->Variables )
    {
        if ( binding->Kind == SyntaxKind::VarDecl )
            GenerateLetBinding( binding.get() );
    }

    GenerateImplicitProgn( &letStmt->Body, config, status );
}

void Compiler::GenerateLetBinding( DataDecl* binding )
{
    auto local = (LocalStorage*) binding->GetDecl();

    GenerateLocalInit( local->Offset, binding->Initializer.get() );
}

void Compiler::GenerateLocalInit( LocalSize offset, Syntax* initializer )
{
    if ( initializer == nullptr )
        return;

    auto type = initializer->Type.get();

    if ( IsScalarType( type->GetKind() ) )
    {
        Generate( initializer );
        EmitU8( OP_STLOC, offset );
        DecreaseExprDepth();
    }
    else if ( type->GetKind() == TypeKind::Array )
    {
        auto arrayType = (ArrayType*) type;

        AddLocalDataArray( offset, initializer, arrayType->Count );
    }
    else
    {
        mRep.ThrowSemanticsError( initializer, "'let' binding takes a name or name and type" );
    }
}

void Compiler::VisitLetStatement( LetStatement* letStmt )
{
    GenerateLet( letStmt, Config(), Status() );
}

void Compiler::AddLocalDataArray( LocalSize offset, Syntax* valueElem, size_t size )
{
    if ( valueElem->Kind != SyntaxKind::ArrayInitializer )
    {
        Generate( valueElem );

        EmitU8( OP_LDLOCA, offset );
        Emit( OP_COPYBLOCK );

        IncreaseExprDepth();
        DecreaseExprDepth( 3 );
        return;
    }

    auto        initList = (InitList*) valueElem;
    LocalSize   locIndex = offset;
    LocalSize   i = 0;

    if ( initList->Values.size() > size )
        mRep.ThrowSemanticsError( valueElem, "Array has too many initializers" );

    for ( auto& entry : initList->Values )
    {
        assert( locIndex+1 >= entry->Type->GetSize() );

        GenerateLocalInit( locIndex, entry.get() );
        i++;
        locIndex -= static_cast<LocalSize>(entry->Type->GetSize());
    }

    if ( initList->Fill == ArrayFill::Extrapolate && initList->Values.size() > 0 )
    {
        size_t count = initList->Values.size();
        I32 prevValue = 0;
        I32 step = 0;

        prevValue = GetSyntaxValue( initList->Values.back().get(), "Array initializer extrapolation requires a constant" );

        if ( initList->Values.size() > 1 )
        {
            auto prevValue2 = GetFinalOptionalSyntaxValue( initList->Values[count - 2].get() );
            if ( prevValue2.has_value() )
                step = prevValue - prevValue2.value();
        }

        for ( ; i < size; i++ )
        {
            I32 newValue = prevValue + step;

            EmitLoadConstant( newValue );
            EmitU8( OP_STLOC, (U8) locIndex );
            locIndex--;
            DecreaseExprDepth();

            prevValue = newValue;
        }
    }
    else if ( initList->Fill == ArrayFill::Repeat && initList->Values.size() > 0 )
    {
        Syntax* lastNode = initList->Values.back().get();

        for ( ; i < size; i++ )
        {
            assert( locIndex+1 >= lastNode->Type->GetSize() );

            GenerateLocalInit( locIndex, lastNode );
            locIndex -= static_cast<LocalSize>(lastNode->Type->GetSize());
        }
    }
}

void Compiler::GenerateCall( CallExpr* call, const GenConfig& config, GenStatus& status )
{
    GenerateCall( call->Head->GetDecl(), call->Arguments, config, status );
}

void Compiler::GenerateCallArgs( std::vector<Unique<Syntax>>& arguments )
{
    for ( auto i = static_cast<ptrdiff_t>( arguments.size() ) - 1; i >= 0; i-- )
    {
        Generate( arguments[i].get() );
    }
}

void Compiler::GenerateCall( Declaration* decl, std::vector<Unique<Syntax>>& arguments, const GenConfig& config, GenStatus& status )
{
    GenerateCallArgs( arguments );

    ParamSize argCount = static_cast<ParamSize>( arguments.size() );
    U8 callFlags = CallFlags::Build( argCount, config.discard );

    if ( decl == nullptr )
    {
        THROW_INTERNAL_ERROR( "Call head has no declaration" );
    }
    else if ( decl->Kind == DeclKind::Func
        && ((Function*) decl)->ModIndex == mModIndex )
    {
        Function* func = (Function*) decl;

        size_t curIndex = ReserveCode( 5 );
        mCodeBin[curIndex+0] = OP_CALL;
        mCodeBin[curIndex+1] = callFlags;

        if ( func->Address == UndefinedAddr )
        {
            PushFuncPatch( func->Name, { CodeRefKind::Code, static_cast<int32_t>( curIndex + 2 ) } );
        }

        StoreU24( &mCodeBin[curIndex+2], func->Address );

        if ( mInFunc )
            mCurFunc->CalledFunctions.push_back( { func->Name, mCurExprDepth, mModIndex } );
    }
    else
    {
        U8  opCode = 0;
        I32 id = 0;

        if ( decl->Kind == DeclKind::Func )
        {
            auto func = (Function*) decl;

            opCode = OP_CALLM;
            id = CodeAddr::Build( func->Address, func->ModIndex );

            mCurFunc->CalledFunctions.push_back( { func->Name, mCurExprDepth, func->ModIndex } );
        }
        else if ( decl->Kind == DeclKind::NativeFunc )
        {
            id = ((NativeFunction*) decl)->Id;

            if ( id >= 0x100 )
                opCode = OP_CALLNATIVE;
            else
                opCode = OP_CALLNATIVE_S;
        }
        else
        {
            THROW_INTERNAL_ERROR( "" );
        }

        if ( opCode == OP_CALLNATIVE_S )
        {
            size_t curIndex = ReserveCode( 3 );
            mCodeBin[curIndex + 0] = opCode;
            mCodeBin[curIndex + 1] = callFlags;
            mCodeBin[curIndex + 2] = (U8) id;
        }
        else
        {
            size_t curIndex = ReserveCode( 6 );
            mCodeBin[curIndex + 0] = opCode;
            mCodeBin[curIndex + 1] = callFlags;

            StoreU32( &mCodeBin[curIndex + 2], id );
        }
    }

    IncreaseExprDepth();

    if ( config.discard )
    {
        status.discarded = true;
        DecreaseExprDepth();
    }

    DecreaseExprDepth( argCount );
}

void Compiler::VisitCallExpr( CallExpr* call )
{
    if ( call->IsIndirect )
        GenerateFuncall( call, Config(), Status() );
    else
        GenerateCall( call, Config(), Status() );
}

void Compiler::GenerateFor( ForStatement* forStmt, const GenConfig& config, GenStatus& status )
{
    // Variable name

    auto local = (LocalStorage*) forStmt->IndexDecl.get();

    U8      primitive;
    int32_t step;

    if ( forStmt->Comparison == ForComparison::Below )
    {
        primitive = PRIM_LT;
        step = 1;
    }
    else if ( forStmt->Comparison == ForComparison::To )
    {
        primitive = PRIM_LE;
        step = 1;
    }
    else if ( forStmt->Comparison == ForComparison::Downto )
    {
        primitive = PRIM_GE;
        step = -1;
    }
    else if ( forStmt->Comparison == ForComparison::Above )
    {
        primitive = PRIM_GT;
        step = -1;
    }
    else
    {
        mRep.ThrowSemanticsError( forStmt, "Expected symbol: to, downto, above, below" );
    }

    PatchChain  bodyChain;
    PatchChain  testChain;
    PatchChain  breakChain;
    PatchChain  nextChain;

    // Beginning expression
    Generate( forStmt->First.get() );
    Emit( OP_DUP );
    EmitU8( OP_STLOC, local->Offset );
    IncreaseExprDepth();
    DecreaseExprDepth();

    // The unconditional jump below leaves one value on the expression stack.
    // Decrease the depth here, because this value overlaps the first one pushed
    // for a usual test.
    DecreaseExprDepth();

    EmitBranch( OP_B, &testChain );

    int32_t bodyLoc = static_cast<int32_t>( mCodeBin.size() );

    // Body
    GenerateStatements( &forStmt->Body, config.WithLoop( &breakChain, &nextChain ), status );

    Patch( &nextChain );

    if ( forStmt->Step != nullptr )
        Generate( forStmt->Step.get() );
    else
        EmitLoadConstant( step );

    EmitU8( OP_LDLOC, local->Offset );
    EmitU8( OP_PRIM, PRIM_ADD );
    Emit( OP_DUP );
    EmitU8( OP_STLOC, local->Offset );
    IncreaseExprDepth();
    DecreaseExprDepth();
    IncreaseExprDepth();
    DecreaseExprDepth();

    Patch( &testChain );

    // Ending expression
    Generate( forStmt->Last.get() );

    EmitU8( OP_PRIM, primitive );
    DecreaseExprDepth();

    EmitBranch( OP_BTRUE, &bodyChain );
    DecreaseExprDepth();

    Patch( &bodyChain, bodyLoc );
    Patch( &breakChain );

    GenerateNilIfNeeded( config, status );
}

void Compiler::VisitForStatement( ForStatement* forStmt )
{
    GenerateFor( forStmt, Config(), Status() );
}

void Compiler::GenerateSimpleLoop( LoopStatement* loopStmt, const GenConfig& config, GenStatus& status )
{
    PatchChain  breakChain;
    PatchChain  nextChain;

    int32_t     bodyLoc = static_cast<int32_t>(mCodeBin.size());

    // Body
    GenerateStatements( &loopStmt->Body, config.WithLoop( &breakChain, &nextChain ), status );

    if ( loopStmt->Condition == nullptr )
    {
        EmitBranch( OP_B, &nextChain );

        Patch( &nextChain, bodyLoc );
    }
    else
    {
        GenStatus exprStatus;
        PatchChain bodyChain;

        Patch( &nextChain );

        Generate( loopStmt->Condition.get(), GenConfig::Expr( &bodyChain, &breakChain, false ), exprStatus );
        ElideFalse( &bodyChain, &breakChain );

        Patch( &bodyChain, bodyLoc );
    }

    Patch( &breakChain );

    GenerateNilIfNeeded( config, status );
}

void Compiler::VisitLoopStatement( LoopStatement* loopStmt )
{
    GenerateSimpleLoop( loopStmt, Config(), Status() );
}

void Compiler::GenerateDo( WhileStatement* whileStmt, const GenConfig& config, GenStatus& status )
{
    PatchChain  breakChain;
    PatchChain  nextChain;
    PatchChain  trueChain;

    int32_t testLoc = static_cast<int32_t>(mCodeBin.size());

    // Test expression
    Generate( whileStmt->Condition.get(), GenConfig::Expr( &trueChain, &breakChain, false ) );

    ElideTrue( &trueChain, &breakChain );
    Patch( &trueChain );

    // Body
    GenerateStatements( &whileStmt->Body, config.WithLoop( &breakChain, &nextChain ), status );

    EmitBranch( OP_B, &nextChain );

    Patch( &breakChain );
    Patch( &nextChain, testLoc );

    GenerateNilIfNeeded( config, status );
}

void Compiler::VisitWhileStatement( WhileStatement* whileStmt )
{
    GenerateDo( whileStmt, Config(), Status() );
}

void Compiler::GenerateBreak( BreakStatement* breakStmt, const GenConfig& config, GenStatus& status )
{
    if ( config.breakChain == nullptr )
        mRep.ThrowSemanticsError( breakStmt, "Cannot use break outside of a loop" );

    EmitBranch( OP_B, config.breakChain );

    status.discarded = true;
}

void Compiler::VisitBreakStatement( BreakStatement* breakStmt )
{
    GenerateBreak( breakStmt, Config(), Status() );
}

void Compiler::GenerateNext( NextStatement* nextStmt, const GenConfig& config, GenStatus& status )
{
    if ( config.nextChain == nullptr )
        mRep.ThrowSemanticsError( nextStmt, "Cannot use next outside of a loop" );

    EmitBranch( OP_B, config.nextChain );

    status.discarded = true;
}

void Compiler::VisitNextStatement( NextStatement* nextStmt )
{
    GenerateNext( nextStmt, Config(), Status() );
}

void Compiler::GenerateCase( CaseExpr* caseExpr, const GenConfig& config, GenStatus& status )
{
    GenerateGeneralCase( caseExpr, config, status );
}

void Compiler::GenerateGeneralCase( CaseExpr* caseExpr, const GenConfig& config, GenStatus& status )
{
    PatchChain  exitChain;
    LocalSize   exprDepth = mCurExprDepth;

    const GenConfig& statementConfig = config;

    for ( auto& clause : caseExpr->Clauses )
    {
        PatchChain falseChain;
        PatchChain trueChain;

        // Restore the expression depth, so that it doesn't accumulate
        mCurExprDepth = exprDepth;

        size_t i = 0;

        for ( auto& key : clause->Keys )
        {
            i++;

            Generate( caseExpr->TestKey.get() );
            Generate( key.get() );

            EmitU8( OP_PRIM, PRIM_EQ );
            DecreaseExprDepth();

            if ( i == clause->Keys.size() )
            {
                EmitBranch( OP_BFALSE, &falseChain );
                DecreaseExprDepth();
            }
            else
            {
                EmitBranch( OP_BTRUE, &trueChain );
                DecreaseExprDepth();
            }
        }

        Patch( &trueChain );

        GenerateImplicitProgn( &clause->Body, statementConfig, status );

        EmitBranch( OP_B, &exitChain );

        Patch( &falseChain );
    }

    // Restore the expression depth, so that it doesn't accumulate
    mCurExprDepth = exprDepth;

    if ( caseExpr->Fallback != nullptr )
    {
        GenerateImplicitProgn( &caseExpr->Fallback->Body, statementConfig, status );
    }
    else
    {
        GenerateNilIfNeeded( config, status );
    }

    Patch( &exitChain );
}

void Compiler::VisitCaseExpr( CaseExpr* caseExpr )
{
    GenerateCase( caseExpr, Config(), Status() );
}

void Compiler::VisitUnaryExpr( UnaryExpr* unary )
{
    if ( unary->Op == "not" )
    {
        Generate( unary->Inner.get(), Config().Invert(), Status() );
        Status().kind = ExprKind::Logical;
    }
    else if ( unary->Op == "-" )
    {
        GenerateUnaryPrimitive( unary->Inner.get(), Config(), Status() );
    }
    else
    {
        THROW_INTERNAL_ERROR( "" );
    }
}

void Compiler::GenerateComparison( BinaryExpr* binary, const GenConfig& config, GenStatus& status )
{
    auto& op = binary->Op;
    U8  positivePrimitive;
    U8  negativePrimitive;

    if ( op == "=" )
    {
        positivePrimitive = PRIM_EQ;
        negativePrimitive = PRIM_NE;
    }
    else if ( op == "<>" )
    {
        positivePrimitive = PRIM_NE;
        negativePrimitive = PRIM_EQ;
    }
    else if ( op == "<" )
    {
        positivePrimitive = PRIM_LT;
        negativePrimitive = PRIM_GE;
    }
    else if ( op == "<=" )
    {
        positivePrimitive = PRIM_LE;
        negativePrimitive = PRIM_GT;
    }
    else if ( op == ">" )
    {
        positivePrimitive = PRIM_GT;
        negativePrimitive = PRIM_LE;
    }
    else if ( op == ">=" )
    {
        positivePrimitive = PRIM_GE;
        negativePrimitive = PRIM_LT;
    }
    else
    {
        THROW_INTERNAL_ERROR( "" );
    }

    GenerateBinaryPrimitive( binary, config.invert ? negativePrimitive : positivePrimitive, config, status );
    status.kind = ExprKind::Comparison;
}

void Compiler::GenerateAnd( BinaryExpr* binary, const GenConfig& config, GenStatus& status )
{
    ConjSpec spec = { &Compiler::GenerateAndClause, &Compiler::GenerateOrClause };
    GenerateConj( &spec, binary, config );
    status.kind = ExprKind::Logical;

    if ( config.discard )
        status.discarded = true;
}

void Compiler::GenerateOr( BinaryExpr* binary, const GenConfig& config, GenStatus& status )
{
    ConjSpec spec = { &Compiler::GenerateOrClause, &Compiler::GenerateAndClause };
    GenerateConj( &spec, binary, config );
    status.kind = ExprKind::Logical;

    if ( config.discard )
        status.discarded = true;
}

void Compiler::GenerateConj( ConjSpec* spec, BinaryExpr* binary, const GenConfig& config )
{
    if ( config.trueChain == nullptr )
    {
        Atomize( spec, binary, config.invert, config.discard );
        return;
    }

    if ( config.invert )
    {
        (this->*(spec->NegativeGenerator))( binary->Left.get(), config );
    }
    else
    {
        (this->*(spec->PositiveGenerator))( binary->Left.get(), config );
    }

    Generate( binary->Right.get(), config );
}

void Compiler::GenerateAndClause( Syntax* elem, const GenConfig& config )
{
    PatchChain  newTrueChain;
    Generate( elem, config.WithTrue( &newTrueChain ) );
    ElideTrue( &newTrueChain, config.falseChain );
    Patch( &newTrueChain );
}

void Compiler::GenerateOrClause( Syntax* elem, const GenConfig& config )
{
    PatchChain  newFalseChain;
    Generate( elem, config.WithFalse( &newFalseChain ) );
    ElideFalse( config.trueChain, &newFalseChain );
    Patch( &newFalseChain );
}

void Compiler::Atomize( ConjSpec* spec, BinaryExpr* binary, bool invert, bool discard )
{
    PatchChain  trueChain;
    PatchChain  falseChain;
    PatchChain  endChain;

    GenerateConj( spec, binary, GenConfig::Expr( &trueChain, &falseChain, invert ) );

    ElideTrue( &trueChain, &falseChain );

    Patch( &trueChain );

    if ( !discard )
    {
        EmitU8( OP_LDC_S, 1 );

        EmitBranch( OP_B, &endChain );
    }

    Patch( &falseChain );

    if ( !discard )
    {
        EmitU8( OP_LDC_S, 0 );

        Patch( &endChain );

        IncreaseExprDepth();
    }
}

U8 Compiler::InvertJump( U8 opCode )
{
    switch ( opCode )
    {
    case OP_BTRUE:  return OP_BFALSE;
    case OP_BFALSE: return OP_BTRUE;
    default:
        THROW_INTERNAL_ERROR( "" );
    }
}

void Compiler::PushPatch( PatchChain* chain )
{
    PushPatch( chain, static_cast<int32_t>(mCodeBin.size()) );
}

void Compiler::PushPatch( PatchChain* chain, int32_t patchLoc )
{
    PushBasicPatch( chain, patchLoc );
}

template <typename TRef>
void Compiler::PushBasicPatch( BasicPatchChain<TRef>* chain, TRef patchLoc )
{
    BasicInstPatch<TRef>* link = new BasicInstPatch<TRef>;
    link->Ref = patchLoc;
    link->Next = chain->First;
    chain->First = link;
}

void Compiler::PopPatch( PatchChain* chain )
{
    assert( chain->First != nullptr );

    auto link = chain->First;
    chain->First = chain->First->Next;
    delete link;
}

void Compiler::PushFuncPatch( const std::string& name, CodeRef codeRef )
{
    auto patchIt = mFuncPatchMap.find( name );
    if ( patchIt == mFuncPatchMap.end() )
    {
        auto result = mFuncPatchMap.insert( FuncPatchMap::value_type( { name, FuncPatchChain() } ) );
        patchIt = std::move( result.first );
    }

    PushBasicPatch( &patchIt->second, codeRef );

    if ( mInFunc )
    {
        AddrRef ref = { AddrRefKind::Inst };
        ref.InstIndexPtr = &patchIt->second.First->Ref.Location;
        mLocalAddrRefs.push_back( ref );
    }
}

void Compiler::ElideTrue( PatchChain* trueChain, PatchChain* falseChain )
{
    if (   trueChain->First  == nullptr
        || falseChain->First == nullptr )
        return;

    int32_t target = static_cast<int32_t>(mCodeBin.size());
    size_t diff = target - (trueChain->First->Ref + BranchInst::Size);

    if ( diff == BranchInst::Size
        && mCodeBin[mCodeBin.size() - BranchInst::Size] == OP_B
        && ((int32_t) mCodeBin.size() - BranchInst::Size) == falseChain->First->Ref
        )
    {
        falseChain->First->Ref = trueChain->First->Ref;

        mCodeBin[trueChain->First->Ref] = InvertJump( mCodeBin[trueChain->First->Ref] );

        // Remove the branch instruction.
        PopPatch( trueChain );

        DeleteCode( BranchInst::Size );
    }
}

void Compiler::ElideFalse( PatchChain* trueChain, PatchChain* falseChain )
{
    if ( falseChain->First == nullptr )
        return;

    int32_t target = static_cast<int32_t>(mCodeBin.size());
    size_t diff = target - (falseChain->First->Ref + BranchInst::Size);

    if ( diff == 0 )
    {
        // Remove the branch instruction.
        PopPatch( falseChain );

        DeleteCode( BranchInst::Size );
    }
}

void Compiler::Patch( PatchChain* chain, int32_t targetIndex )
{
    int32_t target = (targetIndex >= 0) ? targetIndex : static_cast<int32_t>(mCodeBin.size());

    for ( InstPatch* link = chain->First; link != nullptr; link = link->Next )
    {
        ptrdiff_t diff = target - (link->Ref + BranchInst::Size);

        if ( diff < BranchInst::OffsetMin || diff > BranchInst::OffsetMax )
            mRep.ThrowSemanticsError( nullptr, "Branch target is too far." );

        BranchInst::StoreOffset( &mCodeBin[link->Ref + 1], static_cast<BranchInst::TOffset>(diff) );
    }

    chain->PatchedInstIndex = target;
}

void Compiler::PatchCalls( FuncPatchChain* chain, U32 addr )
{
    for ( FuncInstPatch* link = chain->First; link != nullptr; link = link->Next )
    {
        void* refPtr = nullptr;

        switch ( link->Ref.Kind )
        {
        case CodeRefKind::Code: refPtr = &mCodeBin[link->Ref.Location]; break;
        case CodeRefKind::Data: refPtr = &mGlobals[link->Ref.Location]; break;
        default:
            THROW_INTERNAL_ERROR( "" );
        }

        StoreU24( (uint8_t*) refPtr, addr );
    }
}

void Compiler::GenerateUnaryPrimitive( Syntax* elem, const GenConfig& config, GenStatus& status )
{
    if ( config.discard )
    {
        Generate( elem, config, status );
    }
    else
    {
        EmitU8( OP_LDC_S, 0 );

        Generate( elem );

        EmitU8( OP_PRIM, PRIM_SUB );

        IncreaseExprDepth();
        DecreaseExprDepth();
    }
}

void Compiler::GenerateBinaryPrimitive( BinaryExpr* binary, U8 primitive, const GenConfig& config, GenStatus& status )
{
    if ( config.discard )
    {
        GenerateDiscard( binary->Left.get() );
        GenerateDiscard( binary->Right.get() );
        status.discarded = true;
    }
    else
    {
        Generate( binary->Left.get() );
        Generate( binary->Right.get() );

        EmitU8( OP_PRIM, primitive );

        DecreaseExprDepth();
    }
}

void Compiler::EmitLoadAddress( Syntax* node, Declaration* baseDecl, I32 offset )
{
    if ( baseDecl == nullptr )
    {
        THROW_INTERNAL_ERROR( "Missing declaration" );
    }
    else
    {
        uint32_t addrWord;

        switch ( baseDecl->Kind )
        {
        case DeclKind::Global:
            assert( offset >= 0 && offset < GlobalSizeMax );
            assert( offset < (GlobalSizeMax - ((GlobalStorage*) baseDecl)->Offset) );

            addrWord = CodeAddr::Build(
                ((GlobalStorage*) baseDecl)->Offset + offset,
                ((GlobalStorage*) baseDecl)->ModIndex );
            EmitU32( OP_LDC, addrWord );
            IncreaseExprDepth();
            break;

        case DeclKind::Local:
            assert( offset >= 0 && offset < LocalSizeMax );
            assert( offset <= ((LocalStorage*) baseDecl)->Offset );

            EmitU8( OP_LDLOCA, static_cast<uint8_t>(((LocalStorage*) baseDecl)->Offset - offset) );
            IncreaseExprDepth();
            break;

        case DeclKind::LoadedAddress:
            assert( offset >= 0 && offset < DataSizeMax );

            if ( offset > 0 )
                EmitSpilledAddrOffset( offset );
            break;

        default:
            mRep.ThrowSemanticsError( node, "'aref' supports only globals and locals" );
        }
    }
}

void Compiler::GenerateArefAddr( IndexExpr* indexExpr, const GenConfig& config, GenStatus& status )
{
    assert( config.calcAddr );

    auto& arrayType = (ArrayType&) *indexExpr->Head->Type;

    Generate( indexExpr->Head.get(), config, status );

    std::optional<int32_t> optIndexVal;

    optIndexVal = GetFinalOptionalSyntaxValue( indexExpr->Index.get() );

    if ( optIndexVal.has_value() )
    {
        assert( optIndexVal.value() < DataSizeMax );

        status.offset += static_cast<DataSize>(optIndexVal.value()) * arrayType.ElemType->GetSize();
        return;
    }

    if ( !status.spilledAddr )
    {
        EmitLoadAddress( indexExpr, status.baseDecl, status.offset );

        // Set this after emitting the original decl's address above
        status.baseDecl = mLoadedAddrDecl.get();
        status.offset = 0;
        status.spilledAddr = true;
    }

    if ( status.offset > 0 )
    {
        EmitSpilledAddrOffset( status.offset );

        status.offset = 0;
    }

    Generate( indexExpr->Index.get() );

    if ( arrayType.ElemType->GetSize() > UINT8_MAX )
    {
        EmitU32( OP_INDEX, arrayType.ElemType->GetSize() );
    }
    else
    {
        EmitU8( OP_INDEX_S, static_cast<U8>(arrayType.ElemType->GetSize()) );
    }

    DecreaseExprDepth();
}

void Compiler::GenerateAref( IndexExpr* indexExpr, const GenConfig& config, GenStatus& status )
{
    if ( config.discard )
    {
        status.discarded = true;
        return;
    }
    else if ( config.calcAddr )
    {
        GenerateArefAddr( indexExpr, config, status );
        return;
    }

    Declaration* baseDecl = nullptr;
    int32_t offset = 0;

    // Calculate address in each clause below instead of once here,
    // because it might spill. And in the case of array, the spilled address
    // must be right after the size.

    auto& arrayType = (ArrayType&) *indexExpr->Head->Type;
    auto& elemType = *arrayType.ElemType;

    if ( IsScalarType( elemType.GetKind() ) )
    {
        CalcAddress( indexExpr, baseDecl, offset );

        EmitLoadScalar( indexExpr, baseDecl, offset );
    }
    else
    {
        EmitLoadAggregateCopySource( indexExpr, &elemType );
    }
}

void Compiler::VisitIndexExpr( IndexExpr* indexExpr )
{
    GenerateAref( indexExpr, Config(), Status() );
}

void Compiler::CalcAddress( Syntax* expr, Declaration*& baseDecl, int32_t& offset )
{
    GenConfig config{};
    GenStatus status{};

    config.calcAddr = true;

    Generate( expr, config, status );

    baseDecl = status.baseDecl;
    offset = status.offset;
}

void Compiler::VisitSliceExpr( SliceExpr* sliceExpr )
{
    const GenConfig& config = Config();
    GenStatus& status = Status();

    if ( config.discard )
    {
        status.discarded = true;
        return;
    }
    else if ( config.calcAddr )
    {
        Generate( sliceExpr->Head.get(), config, status );

        auto& arrayType = (ArrayType&) *sliceExpr->Head->Type;

        int32_t indexVal = GetSyntaxValue( sliceExpr->FirstIndex.get(), "" );

        assert( indexVal < DataSizeMax );

        status.offset += static_cast<DataSize>(indexVal) * arrayType.ElemType->GetSize();
        return;
    }

    EmitLoadAggregateCopySource( sliceExpr );
}

void Compiler::VisitDotExpr( DotExpr* dotExpr )
{
    GenerateValue( dotExpr, dotExpr->GetDecl(), Config(), Status() );
}

void Compiler::GenerateDefvar( VarDecl* varDecl, const GenConfig& config, GenStatus& status )
{
    auto global = (GlobalStorage*) varDecl->GetDecl();

    GenerateGlobalInit( global->Offset, varDecl->Initializer.get() );
}

void Compiler::GenerateGlobalInit( GlobalSize offset, Syntax* initializer )
{
    if ( initializer == nullptr )
        return;

    auto type = initializer->Type.get();

    if ( IsScalarType( type->GetKind() ) )
    {
        AddGlobalData( offset, initializer );
    }
    else if ( type->GetKind() == TypeKind::Array )
    {
        auto arrayType = (ArrayType*) type;

        AddGlobalDataArray( offset, initializer, arrayType->Count );
    }
    else
    {
        mRep.ThrowSemanticsError( initializer, "'defvar' takes a name or name and type" );
    }
}

void Compiler::VisitVarDecl( VarDecl* varDecl )
{
    GenerateDefvar( varDecl, Config(), Status() );
}

void Compiler::AddGlobalData( GlobalSize offset, Syntax* valueElem )
{
    if ( valueElem->Type->GetKind() == TypeKind::Pointer )
    {
        if ( valueElem->Kind == SyntaxKind::AddrOfExpr )
        {
            auto addrOf = (AddrOfExpr*) valueElem;

            EmitFuncAddress( (Function*) addrOf->Inner->GetDecl(), { CodeRefKind::Data, offset } );
        }
        else
        {
            THROW_INTERNAL_ERROR( "" );
        }
    }
    else
    {
        mGlobals[offset] = GetSyntaxValue( valueElem, "Globals must be initialized with constant data" );
    }
}

void Compiler::AddGlobalDataArray( GlobalSize offset, Syntax* valueElem, size_t size )
{
    if ( valueElem->Kind != SyntaxKind::ArrayInitializer )
    {
        // Defer these globals until all function addresses are known and put in source blocks

        Declaration* baseDecl = nullptr;
        int32_t      srcOffset = 0;
        MemTransfer  transfer;

        CalcAddress( valueElem, baseDecl, srcOffset );

        transfer.Src = srcOffset;
        transfer.Dst = offset;
        transfer.Size = valueElem->Type->GetSize();

        mDeferredGlobals.push_back( transfer );
        return;
    }

    auto        initList = (InitList*) valueElem;
    GlobalSize  i = 0;
    GlobalSize  globalIndex = offset;

    if ( initList->Values.size() > size )
        mRep.ThrowSemanticsError( valueElem, "Array has too many initializers" );

    for ( auto& entry : initList->Values )
    {
        GenerateGlobalInit( globalIndex, entry.get() );
        i++;
        globalIndex += entry->Type->GetSize();
    }

    if ( initList->Fill == ArrayFill::Extrapolate && i >= 1 )
    {
        I32 prevValue = mGlobals[offset + i - 1];
        I32 step = 0;

        if ( i >= 2 )
        {
            step = prevValue - mGlobals[offset + i - 2];
        }

        for ( ; i < size; i++ )
        {
            I32 newValue = prevValue + step;

            mGlobals[offset + i] = newValue;
            prevValue = newValue;
        }
    }
    else if ( initList->Fill == ArrayFill::Repeat && i >= 1 )
    {
        Syntax* lastNode = initList->Values.back().get();

        for ( ; i < size; i++ )
        {
            GenerateGlobalInit( globalIndex, lastNode );
            globalIndex += lastNode->Type->GetSize();
        }
    }
}

void Compiler::GenerateProc( ProcDecl* procDecl, Function* func )
{
    mInFunc = true;
    mCurFunc = func;

    constexpr uint8_t PushInstSize = 2;

    int32_t bodyLoc = static_cast<int32_t>(mCodeBin.size());

    // Assume that there are local variables
    EmitU8( OP_PUSH, 0 );

    mCurExprDepth = 0;
    mMaxExprDepth = 0;
    mLocalAddrRefs.clear();

    GenConfig config = GenConfig::Statement();
    GenStatus status = { ExprKind::Other };

    mGenStack.push_back( { config, status } );

    procDecl->Body.Accept( this );

    mGenStack.pop_back();

    if ( !status.tailRet )
    {
        Emit( OP_RET );
    }

    if ( func->LocalCount > 0 )
    {
        size_t index = bodyLoc + 1;
        mCodeBin[index] = (uint8_t) func->LocalCount;
    }
    else
    {
        // No locals. So, delete the PUSH instruction
        DeleteCode( bodyLoc, PushInstSize );

        // If local lambda references were generated, then shift them
        // This also includes references to any function

        for ( const auto& ref : mLocalAddrRefs )
        {
            int32_t* instIndexPtr = nullptr;

            switch ( ref.Kind )
            {
            case AddrRefKind::Inst:
                instIndexPtr = ref.InstIndexPtr;
                break;

            default:
                THROW_INTERNAL_ERROR( "" );
            }

            *instIndexPtr -= PushInstSize;
        }
    }

    func->ExprDepth = mMaxExprDepth;

    mCurFunc = nullptr;
    mInFunc = false;
}

void Compiler::GenerateImplicitProgn( StatementList* stmtList, const GenConfig& config, GenStatus& status )
{
    for ( int i = 0; i < (int) stmtList->Statements.size() - 1; i++ )
    {
        GenerateDiscard( stmtList->Statements[i].get() );
    }

    if ( stmtList->Statements.size() >= 1 )
    {
        Generate( stmtList->Statements.back().get(), config, status );
    }
    else    // There are no expressions
    {
        GenerateNilIfNeeded( config, status );
    }
}

void Compiler::VisitStatementList( StatementList* stmtList )
{
    GenerateImplicitProgn( stmtList, Config(), Status() );
}

void Compiler::GenerateStatements( StatementList* list, const GenConfig& config, GenStatus& status )
{
    for ( auto& node : list->Statements )
    {
        GenerateDiscard( node.get(), config );
    }
}

void Compiler::GenerateNilIfNeeded( const GenConfig& config, GenStatus& status )
{
    if ( config.discard )
    {
        status.discarded = true;
    }
    else
    {
        EmitU8( OP_LDC_S, 0 );

        IncreaseExprDepth();
    }
}

const Compiler::GenConfig& Compiler::Config() const
{
    assert( mGenStack.size() >= 1 );
    return mGenStack.back().config;
}

Compiler::GenStatus& Compiler::Status()
{
    assert( mGenStack.size() >= 1 );
    return mGenStack.back().status;
}

void Compiler::GenerateSentinel()
{
    size_t curIndex = ReserveProgram( SENTINEL_SIZE );

    for ( int i = 0; i < SENTINEL_SIZE; i++ )
    {
        mCodeBin[curIndex++] = OP_SENTINEL;
    }
}

I32 Compiler::GetSyntaxValue( Syntax* node, const char* message )
{
    auto optValue = GetFinalOptionalSyntaxValue( node );

    if ( optValue.has_value() )
        return optValue.value();

    if ( message != nullptr )
        mRep.ThrowSemanticsError( node, message );
    else
        mRep.ThrowSemanticsError( node, "Expected a constant value" );
}

void Compiler::IncreaseExprDepth()
{
    if ( mCurExprDepth == LocalSizeMax )
        mRep.ThrowSemanticsError( NULL, "Expression is too deep" );

    mCurExprDepth++;

    if ( mMaxExprDepth < mCurExprDepth )
        mMaxExprDepth = mCurExprDepth;
}

void Compiler::DecreaseExprDepth( LocalSize amount )
{
    assert( amount >= 0 );
    assert( amount <= mCurExprDepth );

    mCurExprDepth -= amount;
}

void Compiler::CopyDeferredGlobals()
{
    for ( const auto& transfer : mDeferredGlobals )
    {
        std::copy_n( &mGlobals[transfer.Src], transfer.Size, &mGlobals[transfer.Dst] );
    }
}

void Compiler::CalculateStackDepth()
{
    mStats.Lambda = {};
    mStats.Static = {};
    mStats.CallsIndirectly = false;

    for ( const auto& [name, decl] : mGlobalTable )
    {
        if ( decl->Kind == DeclKind::Func )
        {
            Function*  func = (Function*) decl.get();
            CallStats* callStats;

            CalculateStackDepth( func );

            if ( func->IsLambda )
            {
                callStats = &mStats.Lambda;
            }
            else
            {
                callStats = &mStats.Static;

                if ( func->CallsIndirectly )
                    mStats.CallsIndirectly = true;
            }

            // Only count parameters of publics, if that's ever a distinction

            uint32_t stackUsage = func->TreeStackUsage + func->ParamCount;

            callStats->MaxCallDepth  = std::max( callStats->MaxCallDepth,  func->CallDepth );
            callStats->MaxStackUsage = std::max( callStats->MaxStackUsage, stackUsage );

            if ( func->IsRecursive )
                callStats->Recurses = true;
        }
    }
}

void Compiler::CalculateStackDepth( Function* func )
{
    if ( func->IsDepthKnown )
        return;

    if ( func->IsCalculating )
    {
        func->IsRecursive = true;
        return;
    }

    // TODO: Put Machine::FRAME_WORDS somewhere common, and use it here.

    func->IsCalculating = true;
    func->IndividualStackUsage = 2 + func->LocalCount;

    uint32_t maxChildDepth = 0;
    uint32_t maxChildStackUsage = func->ExprDepth;

    for ( const auto& site : func->CalledFunctions )
    {
        SymTable* table = nullptr;

        if ( site.ModIndex == mModIndex )
        {
            table = &mGlobalTable;
        }
        else
        {
            if ( auto modIt = mModulesById.find( site.ModIndex );
                modIt != mModulesById.end() )
            {
                table = &modIt->second->Table;
            }
        }

        if ( table == nullptr )
            continue;

        if ( auto it = table->find( site.FunctionName );
            it != table->end() )
        {
            if ( it->second->Kind != DeclKind::Func )
                continue;

            auto childFunc = (Function*) it->second.get();

            CalculateStackDepth( childFunc );

            uint32_t childStackUsage = childFunc->TreeStackUsage + site.ExprDepth;

            maxChildDepth = std::max( maxChildDepth, childFunc->CallDepth );
            maxChildStackUsage = std::max( maxChildStackUsage, childStackUsage );

            if ( childFunc->CallsIndirectly )
                func->CallsIndirectly = true;

            if ( childFunc->IsRecursive )
            {
                func->IsRecursive = true;
                break;
            }
        }
    }

    func->IsCalculating = false;
    func->IsDepthKnown = true;
    func->CallDepth = 1 + maxChildDepth;
    func->TreeStackUsage = func->IndividualStackUsage + maxChildStackUsage;
}

size_t Compiler::ReserveProgram( size_t size )
{
    if ( size > (CodeSizeMax - mCodeBin.size()) )
        mRep.ThrowSemanticsError( NULL, "Generated code is too big. Max=%u", CodeSizeMax );

    size_t curIndex = mCodeBin.size();
    mCodeBin.resize( mCodeBin.size() + size );

    return curIndex;
}

size_t Compiler::ReserveCode( size_t size )
{
    assert( mInFunc );
    return ReserveProgram( size );
}

void Compiler::DeleteCode( size_t size )
{
    assert( mCodeBin.size() >= size );

    mCodeBin.resize( mCodeBin.size() - size );
}

void Compiler::DeleteCode( size_t start, size_t size )
{
    assert( start < mCodeBin.size() && size <= (mCodeBin.size() - start) );

    mCodeBin.erase( mCodeBin.begin() + start, mCodeBin.begin() + start + size );
}

void Compiler::EmitBranch( OpCode opcode, PatchChain* chain )
{
    PushPatch( chain );

    size_t curIndex = ReserveCode( BranchInst::Size );

    mCodeBin[curIndex] = opcode;
}

void Compiler::Emit( OpCode opcode )
{
    size_t curIndex = ReserveCode( 1 );

    mCodeBin[curIndex] = opcode;
}

void Compiler::EmitU8( OpCode opcode, U8 operand )
{
    size_t curIndex = ReserveCode( 2 );

    mCodeBin[curIndex] = opcode;
    mCodeBin[curIndex + 1] = operand;
}

void Compiler::EmitU16( OpCode opcode, U16 operand )
{
    size_t curIndex = ReserveCode( 3 );

    mCodeBin[curIndex] = opcode;

    StoreU16( &mCodeBin.at( curIndex + 1 ), operand );
}

void Compiler::EmitU32( OpCode opcode, U32 operand )
{
    size_t curIndex = ReserveCode( 5 );

    mCodeBin[curIndex] = opcode;

    StoreU32( &mCodeBin.at( curIndex + 1 ), operand );
}

void Compiler::EmitModAccess( OpCode opcode, U8 mod, U16 addr )
{
    size_t curIndex = ReserveCode( 4 );

    mCodeBin[curIndex+0] = opcode;
    mCodeBin[curIndex+1] = mod;

    StoreU16( &mCodeBin[curIndex + 2], addr );
}


//----------------------------------------------------------------------------

void Log( ICompilerLog* log, LogCategory category, const char* fileName, int line, int col, const char* format, va_list args );


Reporter::Reporter( ICompilerLog* log ) :
    mLog( log )
{
    assert( log != nullptr );
}

ICompilerLog* Reporter::GetLog()
{
    return mLog;
}

void Reporter::ThrowError( CompilerErr exceptionCode, Syntax* elem, const char* format, va_list args )
{
    const char* fileName = nullptr;
    int line = 0;
    int column = 0;
    if ( elem != nullptr )
    {
        fileName = elem->FileName;
        line = elem->Line;
        column = elem->Column;
    }
    ThrowError( exceptionCode, fileName, line, column, format, args );
}

void Reporter::ThrowError( CompilerErr exceptionCode, const char* fileName, int line, int col, const char* format, va_list args )
{
    Log( LogCategory::ERROR, fileName, line, col, format, args );
    throw CompilerException( exceptionCode );
}

void Reporter::ThrowSemanticsError( Syntax* node, const char* format, ... )
{
    va_list args;
    va_start( args, format );
    ThrowError( CompilerErr::SEMANTICS, node, format, args );
    // No need to run va_end( args ), since an exception was thrown
}

void Reporter::ThrowInternalError( const char* fileName, int line, const char* format, ... )
{
    va_list args;
    va_start( args, format );
    ThrowError( CompilerErr::INTERNAL, fileName, line, 1, format, args );
    // No need to run va_end( args ), since an exception was thrown
}

void Reporter::Log( LogCategory category, const char* fileName, int line, int col, const char* format, va_list args )
{
    Gemini::Log( mLog, category, fileName, line, col, format, args );
}

void Reporter::LogWarning( const char* fileName, int line, int col, const char* format, ... )
{
    va_list args;
    va_start( args, format );
    Log( LogCategory::WARNING, fileName, line, col, format, args );
    va_end( args );
}


void Log( ICompilerLog* log, LogCategory category, const char* fileName, int line, int col, const char* format, va_list args )
{
    if ( log != nullptr )
    {
        char msg[256] = "";
        vsnprintf( msg, sizeof msg, format, args );
        log->Add( category, fileName, line, col, msg );
    }
}

}

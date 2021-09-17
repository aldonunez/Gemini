// Gemini Languages and Virtual Machine
// Copyright 2021 Aldo Jose Nunez
//
// Licensed under the Apache License, Version 2.0.
// See the LICENSE.txt file for details.

#pragma once

#include "LangCommon.h"
#include "Syntax.h"
#include <optional>


namespace Gemini
{

class FolderVisitor final : public Visitor
{
    std::optional<int32_t>  mLastValue;

    bool                    mCalcOffset = false;
    std::optional<int32_t>  mBufOffset;
    std::shared_ptr<std::vector<int32_t>> mBuffer;

    bool                    mFoldNodes;
    Reporter                mRep;

    std::shared_ptr<IntType>    mIntType;

    ConstIndexFuncMap*      mConstIndexFuncMap;

public:
    FolderVisitor( ICompilerLog* log, ConstIndexFuncMap* constIndexFuncMap = nullptr );

    std::optional<int32_t> Evaluate( Syntax* node );
    void Fold( Syntax* node );

    // Visitor
    virtual void VisitAddrOfExpr( AddrOfExpr* addrOf ) override;
    virtual void VisitArrayTypeRef( ArrayTypeRef* typeRef ) override;
    virtual void VisitAsExpr( AsExpr* asExpr ) override;
    virtual void VisitAssignmentExpr( AssignmentExpr* assignment ) override;
    virtual void VisitBinaryExpr( BinaryExpr* binary ) override;
    virtual void VisitBreakStatement( BreakStatement* breakStmt ) override;
    virtual void VisitCallExpr( CallExpr* call ) override;
    virtual void VisitCallOrSymbolExpr( CallOrSymbolExpr* callOrSymbol ) override;
    virtual void VisitCaseExpr( CaseExpr* caseExpr ) override;
    virtual void VisitCondExpr( CondExpr* condExpr ) override;
    virtual void VisitConstDecl( ConstDecl* constDecl ) override;
    virtual void VisitCountofExpr( CountofExpr* countofExpr ) override;
    virtual void VisitDotExpr( DotExpr* dotExpr ) override;
    virtual void VisitForStatement( ForStatement* forStmt ) override;
    virtual void VisitIndexExpr( IndexExpr* indexExpr ) override;
    virtual void VisitInitList( InitList* initList ) override;
    virtual void VisitLambdaExpr( LambdaExpr* lambdaExpr ) override;
    virtual void VisitLetStatement( LetStatement* letStmt ) override;
    virtual void VisitLoopStatement( LoopStatement* loopStmt ) override;
    virtual void VisitNameExpr( NameExpr* nameExpr ) override;
    virtual void VisitNextStatement( NextStatement* nextStmt ) override;
    virtual void VisitNumberExpr( NumberExpr* numberExpr ) override;
    virtual void VisitParamDecl( ParamDecl* paramDecl ) override;
    virtual void VisitProcDecl( ProcDecl* procDecl ) override;
    virtual void VisitRecordInitializer( RecordInitializer* recordInitializer ) override;
    virtual void VisitReturnStatement( ReturnStatement* retStmt ) override;
    virtual void VisitSliceExpr( SliceExpr* sliceExpr ) override;
    virtual void VisitStatementList( StatementList* stmtList ) override;
    virtual void VisitUnaryExpr( UnaryExpr* unary ) override;
    virtual void VisitUnit( Unit* unit ) override;
    virtual void VisitVarDecl( VarDecl* varDecl ) override;
    virtual void VisitWhileStatement( WhileStatement* whileStmt ) override;

private:
    void VisitProc( ProcDecl* procDecl );
    void VisitLetBinding( DataDecl* varDecl );
    void VisitFieldAccess( DotExpr* dotExpr );

    void Fold( Unique<Syntax>& child );
};


class FuncAddrVisitor final : public Visitor
{
    Reporter                    mRep;
    std::shared_ptr<Function>   mLastValue;

public:
    FuncAddrVisitor( ICompilerLog* log );

    std::shared_ptr<Function> Evaluate( Syntax* node );

    // Visitor
    virtual void VisitAddrOfExpr( AddrOfExpr* addrOf ) override;
    virtual void VisitDotExpr( DotExpr* dotExpr ) override;
    virtual void VisitNameExpr( NameExpr* nameExpr ) override;

    void VisitDotOrNameExpr( Syntax* expr );
};

}

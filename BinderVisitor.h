#pragma once

#include "Syntax.h"
#include "Compiler.h"


class BinderVisitor : public IVisitor
{
    using SymTable = Compiler::SymTable;
    using SymStack = std::vector<Compiler::SymTable*>;

    // This relies on the syntax nodes sticking around outside of its control.
    // But since shared_ptr is used, instead of internal ref counts,
    // this is not robust.

    using LambdaVec = std::vector<LambdaExpr*>;

    friend class LocalScope;

    LambdaVec       mLambdas;
    SymStack        mSymStack;
    SymTable        mExtTable;
    SymTable&       mConstTable;
    SymTable&       mGlobalTable;
    ICompilerEnv*   mEnv;
    Reporter        mRep;

    int             mCurLevelLocalCount;
    int             mCurLocalCount;
    int             mMaxLocalCount;
    int             mGlobalSize;

public:
    BinderVisitor(
        SymTable& constTable,
        SymTable& globalTable,
        ICompilerEnv* env,
        ICompilerLog* log );

    void Bind( Unit* unit );

    size_t GetDataSize();

    // IVisitor
    virtual void VisitAddrOfExpr( AddrOfExpr* addrOf ) override;
    virtual void VisitArrayTypeRef( ArrayTypeRef* typeRef ) override;
    virtual void VisitAssignmentExpr( AssignmentExpr* assignment ) override;
    virtual void VisitBinaryExpr( BinaryExpr* binary ) override;
    virtual void VisitBreakStatement( BreakStatement* breakStmt ) override;
    virtual void VisitCallExpr( CallExpr* call ) override;
    virtual void VisitCallOrSymbolExpr( CallOrSymbolExpr* callOrSymbol ) override;
    virtual void VisitCaseExpr( CaseExpr* caseExpr ) override;
    virtual void VisitCondExpr( CondExpr* condExpr ) override;
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
    virtual void VisitReturnStatement( ReturnStatement* retStmt ) override;
    virtual void VisitStatementList( StatementList* stmtList ) override;
    virtual void VisitUnaryExpr( UnaryExpr* unary ) override;
    virtual void VisitUnit( Unit* unit ) override;
    virtual void VisitVarDecl( VarDecl* varDecl ) override;
    virtual void VisitWhileStatement( WhileStatement* whileStmt ) override;

private:
    void BindLambdas();

    void VisitProc( ProcDecl* procDecl );
    void VisitLetBinding( VarDecl* varDecl );

    I32 GetElementValue( Syntax* elem, const char* message = nullptr );

    // Symbol table
    std::shared_ptr<Declaration> FindSymbol( const std::string& symbol );
    std::shared_ptr<Storage> AddArg( const std::string& name );
    std::shared_ptr<Storage> AddLocal( SymTable& table, const std::string& name, int offset );
    std::shared_ptr<Storage> AddLocal( const std::string& name, size_t size );
    std::shared_ptr<Storage> AddGlobal( const std::string& name, size_t size );
    std::shared_ptr<Function> AddFunc( const std::string& name, int address );
};
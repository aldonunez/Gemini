#pragma once

#include "GeminiCommon.h"
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <optional>
#include <unordered_map>
#include "Syntax.h"


enum CompilerErr
{
    CERR_OK,
    CERR_INTERNAL,
    CERR_UNSUPPORTED,
    CERR_SYNTAX,
    CERR_SEMANTICS,
};

enum ExternalKind
{
    External_Bytecode,
    External_Native,
};

struct ExternalFunc
{
    int             Id;
    int             Address;
    ExternalKind    Kind;
};

class ICompilerEnv
{
public:
    virtual bool AddExternal( const std::string& name, ExternalKind kind, int address ) = 0;
    virtual bool FindExternal( const std::string& name, ExternalFunc* func ) = 0;
    virtual bool AddGlobal( const std::string& name, int offset ) = 0;
    virtual bool FindGlobal( const std::string& name, int& offset ) = 0;
};

enum LogCategory
{
    LOG_ERROR,
    LOG_WARNING,
};

class ICompilerLog
{
public:
    virtual void Add( LogCategory category, int line, int column, const char* message ) = 0;
};

struct CallStats
{
    int16_t     MaxCallDepth;
    int16_t     MaxStackUsage;
    bool        Recurses;
};

struct CompilerStats
{
    int         CodeBytesWritten;
    bool        CallsIndirectly;
    CallStats   Lambda;
    CallStats   Static;
};

class LocalScope;


class Compiler : public IVisitor
{
public:
    class CompilerException : public std::exception
    {
        CompilerErr     mError;

    public:
        CompilerException( CompilerErr error )
            :   mError( error )
        {
        }

        CompilerErr GetError() const
        {
            return mError;
        }
    };

private:
    friend class LocalScope;

    struct InstPatch
    {
        InstPatch*  Next;
        U8*         Inst;
    };

    struct PatchChain
    {
        InstPatch*  First;

        PatchChain()
            :   First( nullptr )
        {
        }

        ~PatchChain()
        {
            while ( First != nullptr )
            {
                auto link = First;
                First = First->Next;
                delete link;
            }
        }
    };

    enum class DeclKind
    {
        Const,
        Global,
        Local,
        Arg,
        Func,
        Forward,
    };

    struct Declaration
    {
        DeclKind  Kind;
        virtual ~Declaration() { }
    };

    struct ConstDecl : public Declaration
    {
        int Value;
    };

    struct Storage : public Declaration
    {
        int Offset;
    };

    struct Function : public Declaration
    {
        std::string Name;
        int         Address;
        PatchChain  Patches;

        int16_t     LocalCount;
        int16_t     ArgCount;
        int16_t     ExprDepth;

        int16_t     CallDepth;
        int16_t     IndividualStackUsage;
        int16_t     TreeStackUsage;

        bool        IsCalculating;
        bool        IsRecursive;
        bool        IsDepthKnown;
        bool        CallsIndirectly;

        std::list<std::string> CalledFunctions;
    };

    struct DeferredLambda
    {
        ProcDecl*   Definition;
        U8*         Patch;
    };

    enum class AddrRefKind
    {
        Lambda,
        Inst,
    };

    struct AddrRef
    {
        AddrRefKind Kind;
        union
        {
            size_t  LambdaIndex;
            U8**    InstPtr;
        };
    };

    enum class ExprKind
    {
        Other,
        Logical,
        Comparison,
    };

    struct GenStatus
    {
        ExprKind    kind;
        bool        discarded;
        bool        tailRet;
    };

    struct GenConfig
    {
        PatchChain* trueChain;
        PatchChain* falseChain;
        bool invert;
        bool discard;
        PatchChain* breakChain;
        PatchChain* nextChain;

        static GenConfig Discard()
        {
            GenConfig config = { nullptr, nullptr, false, true };
            return config;
        }

        static GenConfig Statement( bool discard = false )
        {
            GenConfig config = { nullptr, nullptr, false, discard };
            return config;
        }

        static GenConfig Expr( PatchChain* trueChain, PatchChain* falseChain, bool invert )
        {
            GenConfig config = { trueChain, falseChain, invert };
            return config;
        }

        GenConfig Invert() const
        {
            GenConfig config = { trueChain, falseChain, !invert, discard };
            return config;
        }

        GenConfig WithFalse( PatchChain* chain ) const
        {
            GenConfig config = { trueChain, chain, invert };
            return config;
        }

        GenConfig WithTrue( PatchChain* chain ) const
        {
            GenConfig config = { chain, falseChain, invert };
            return config;
        }

        GenConfig WithLoop( PatchChain* breakChain, PatchChain* nextChain ) const
        {
            GenConfig config = *this;
            config.breakChain = breakChain;
            config.nextChain = nextChain;
            return config;
        }

        GenConfig WithDiscard() const
        {
            GenConfig config = *this;
            config.discard = true;
            return config;
        }
    };

    typedef void (Compiler::*ConjClauseGenerator)( Syntax* elem, const GenConfig& config );

    struct ConjSpec
    {
        ConjClauseGenerator PositiveGenerator;
        ConjClauseGenerator NegativeGenerator;
    };

    typedef std::map<std::string, std::unique_ptr<Declaration>> SymTable;
    typedef std::vector<SymTable*> SymStack;
    typedef std::vector<DeferredLambda> LambdaVec;
    typedef std::vector<AddrRef> AddrRefVec;

    using GlobalVec = std::vector<I32>;

    struct GenParams
    {
        const GenConfig& config;
        GenStatus& status;
    };

    U8*             mCodeBin;
    U8*             mCodeBinPtr;
    U8*             mCodeBinEnd;
    GlobalVec       mGlobals;

    SymTable        mConstTable;
    SymTable        mGlobalTable;
    SymStack        mSymStack;
    LambdaVec       mLambdas;
    AddrRefVec      mLocalAddrRefs;
    int             mCurLevelLocalCount;
    int             mCurLocalCount;
    int             mMaxLocalCount;
    int             mForwards;
    bool            mInFunc;
    Function*       mCurFunc;
    int16_t         mCurExprDepth;
    int16_t         mMaxExprDepth;

    ICompilerEnv*   mEnv;
    ICompilerLog*   mLog;
    int             mModIndex;

    std::vector<GenParams> mGenStack;

    bool            mCompiled;
    bool            mCalculatedStats;
    CompilerStats   mStats;

public:
    Compiler( U8* codeBin, int codeBinLen, ICompilerEnv* env, ICompilerLog* log, int modIndex = 0 );

    CompilerErr Compile( Unit* progTree );
    void GetStats( CompilerStats& stats );
    I32* GetData();
    size_t GetDataSize();

private:
    // Code generation

    const GenConfig& Config() const;
    GenStatus& Status();

    // Level 1
    void Generate( Syntax* elem );
    void Generate( Syntax* elem, const GenConfig& config );
    void Generate( Syntax* elem, const GenConfig& config, GenStatus& status );
    void GenerateDiscard( Syntax* elem );
    void GenerateDiscard( Syntax* elem, const GenConfig& config );

    // Level 2 - S-expressions
    void GenerateNumber( NumberExpr* number, const GenConfig& config, GenStatus& status );
    void GenerateSymbol( NameExpr* symbol, const GenConfig& config, GenStatus& status );
    void GenerateEvalStar( CallOrSymbolExpr* callOrSymbol, const GenConfig& config, GenStatus& status );
    void GenerateAref( IndexExpr* indexExpr, const GenConfig& config, GenStatus& status );
    void GenerateArrayElementRef( IndexExpr* indexExpr );
    void GenerateDefvar( VarDecl* varDecl, const GenConfig& config, GenStatus& status );

    void AddGlobalData( U32 offset, Syntax* valueElem );
    void AddGlobalDataArray( Storage* global, Syntax* valueElem, size_t size );

    void EmitLoadConstant( int32_t value );

    // Level 3 - functions and special operators
    void GenerateArithmetic( BinaryExpr* binary, const GenConfig& config, GenStatus& status );
    void GenerateComparison( BinaryExpr* binary, const GenConfig& config, GenStatus& status );
    void GenerateAnd( BinaryExpr* binary, const GenConfig& config, GenStatus& status );
    void GenerateOr( BinaryExpr* binary, const GenConfig& config, GenStatus& status );
    void GenerateReturn( ReturnStatement* retStmt, const GenConfig& config, GenStatus& status );
    void GenerateCond( CondExpr* condExpr, const GenConfig& config, GenStatus& status );
    void GenerateSet( AssignmentExpr* assignment, const GenConfig& config, GenStatus& status );
    void GenerateLambda( LambdaExpr* lambdaExpr, const GenConfig& config, GenStatus& status );
    void GenerateFunction( AddrOfExpr* addrOf, const GenConfig& config, GenStatus& status );
    void GenerateFuncall( CallExpr* call, const GenConfig& config, GenStatus& status );
    void GenerateLet( LetStatement* letStmt, const GenConfig& config, GenStatus& status );
    void GenerateLetBinding( VarDecl* binding );
    void AddLocalDataArray( Storage* global, Syntax* valueElem, size_t size );

    void GenerateCall( CallExpr* call, const GenConfig& config, GenStatus& status );
    void GenerateFor( ForStatement* forStmt, const GenConfig& config, GenStatus& status );
    void GenerateSimpleLoop( LoopStatement* loopStmt, const GenConfig& config, GenStatus& status );
    void GenerateDo( WhileStatement* whileStmt, const GenConfig& config, GenStatus& status );
    void GenerateBreak( BreakStatement* breakStmt, const GenConfig& config, GenStatus& status );
    void GenerateNext( NextStatement* nextStmt, const GenConfig& config, GenStatus& status );
    void GenerateCase( CaseExpr* caseExpr, const GenConfig& config, GenStatus& status );
    void GenerateGeneralCase( CaseExpr* caseExpr, const GenConfig& config, GenStatus& status );

    void GenerateUnaryPrimitive( Syntax* elem, const GenConfig& config, GenStatus& status );
    void GenerateBinaryPrimitive( BinaryExpr* binary, int primitive, const GenConfig& config, GenStatus& status );

    void GenerateLambdas();
    void GenerateProc( ProcDecl* procDecl, Function* func );
    void GenerateImplicitProgn( StatementList* stmtList, const GenConfig& config, GenStatus& status );
    void GenerateStatements( StatementList* list, const GenConfig& config, GenStatus& status );
    void GenerateNilIfNeeded( const GenConfig& config, GenStatus& status );

    void GenerateSentinel();

    // And and Or
    void GenerateConj( ConjSpec* spec, BinaryExpr* binary, const GenConfig& config );
    void GenerateAndClause( Syntax* elem, const GenConfig& config );
    void GenerateOrClause( Syntax* elem, const GenConfig& config );
    void Atomize( ConjSpec* spec, BinaryExpr* binary, bool invert, bool discard );

    // And and Or plumbing
    void ElideTrue( PatchChain* trueChain, PatchChain* falseChain );
    void ElideFalse( PatchChain* trueChain, PatchChain* falseChain );
    U8 InvertJump( U8 opCode );

    // Backpatching
    void Patch( PatchChain* chain, U8* targetPtr = nullptr );
    void PatchCalls( PatchChain* chain, U32 addr );
    void PushPatch( PatchChain* chain );
    void PopPatch( PatchChain* chain );

    // Symbol table
    Declaration* FindSymbol( const std::string& symbol );
    Storage* AddArg( SymTable& table, const std::string& name, int offset );
    Storage* AddLocal( SymTable& table, const std::string& name, int offset );
    Storage* AddLocal( const std::string& name, size_t size );
    Storage* AddGlobal( const std::string& name, size_t size );
    Function* AddFunc( const std::string& name, int address );
    Function* AddForward( const std::string& name );
    ConstDecl* AddConst( const std::string& name, int value );
    void MakeStdEnv();
    void CollectFunctionForwards( Unit* program );

    I32 GetElementValue( Syntax* elem, const char* message = nullptr );
    std::optional<I32> GetOptionalElementValue( Syntax* elem );

    // Stack usage
    void IncreaseExprDepth();
    void DecreaseExprDepth( int amount = 1 );
    void CalculateStackDepth();
    void CalculateStackDepth( Function* func );

    [[noreturn]] void ThrowError( CompilerErr exceptionCode, Syntax* elem, const char* format, ... );
    [[noreturn]] void ThrowError( CompilerErr exceptionCode, int line, int col, const char* format, va_list args );
    [[noreturn]] void ThrowInternalError();
    [[noreturn]] void ThrowInternalError( const char* format, ... );
    [[noreturn]] void ThrowUnresolvedFuncsError();

    void Log( LogCategory category, int line, int col, const char* format, va_list args );
    void LogWarning( int line, int col, const char* format, ... );


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
};


void Log( ICompilerLog* log, LogCategory category, int line, int col, const char* format, va_list args );

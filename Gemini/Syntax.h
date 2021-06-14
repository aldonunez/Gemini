// Gemini Languages and Virtual Machine
// Copyright 2021 Aldo Jose Nunez
//
// Licensed under the Apache License, Version 2.0.
// See the LICENSE.txt file for details.

#pragma once

#include <list>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>


namespace Gemini
{

enum class ScopeKind
{
    Global,
    Local,
};


enum class SyntaxKind
{
    Number,
    Name,
    AddrOfExpr,
    Index,
    DotExpr,
    ArrayInitializer,
    ConstDecl,
    VarDecl,
    Other,
};


using CodeSize      = uint_least32_t;
using GlobalSize    = uint_least16_t;
using LocalSize     = uint_least8_t;
using ParamSize     = uint_least8_t;
using DataSize      = GlobalSize;
using ModSize       = uint_least8_t;

constexpr CodeSize      CodeSizeMax = 16777215;
constexpr GlobalSize    GlobalSizeMax = 65535;
constexpr LocalSize     LocalSizeMax = 255;
constexpr ParamSize     ParamSizeMax = 127;
constexpr DataSize      DataSizeMax = GlobalSizeMax;
constexpr ModSize       ModSizeMax = 126;

constexpr CodeSize      UndefinedAddr = 16777215;


class Visitor;

class Syntax;
class ProcDecl;

struct Declaration;

class Type;


template <class T = Syntax>
using Unique = std::unique_ptr<T, std::default_delete<Syntax>>;


class Syntax
{
public:
    SyntaxKind Kind = SyntaxKind::Other;
    int Line = 0;
    int Column = 0;
    const char* FileName = nullptr;

    // All nodes in the same syntax tree refer to the file name string in the root Unit

    std::shared_ptr<Gemini::Type>   Type;

    virtual ~Syntax() {}
    virtual void Accept( Visitor* visitor ) = 0;
    virtual Declaration* GetDecl();
};

class StatementList : public Syntax
{
public:
    std::vector<Unique<Syntax>> Statements;

    virtual void Accept( Visitor* visitor ) override;
};

class NameExpr : public Syntax
{
public:
    std::shared_ptr<Declaration> Decl;
    std::string String;

    NameExpr();
    NameExpr( const std::string& str );
    NameExpr( std::string&& str );

    virtual void Accept( Visitor* visitor ) override;
    virtual Declaration* GetDecl() override;
};

class NumberExpr : public Syntax
{
public:
    int64_t Value;

    NumberExpr();
    NumberExpr( int64_t value );

    virtual void Accept( Visitor* visitor ) override;
};

class TypeRef : public Syntax
{
public:
    std::shared_ptr<Gemini::Type> ReferentType;
};

class NameTypeRef : public TypeRef
{
public:
    Unique<Syntax>  QualifiedName;

    virtual void Accept( Visitor* visitor ) override;
};

class ArrayTypeRef : public TypeRef
{
public:
    Unique<Syntax>  SizeExpr;
    Unique<TypeRef> ElementTypeRef;

    virtual void Accept( Visitor* visitor ) override;
};

class ProcTypeRef : public TypeRef
{
public:
    std::vector<Unique<TypeRef>> Params;
    Unique<TypeRef>              ReturnTypeRef;

    virtual void Accept( Visitor* visitor ) override;
};

class PointerTypeRef : public TypeRef
{
public:
    Unique<TypeRef> Target;

    virtual void Accept( Visitor* visitor ) override;
};

enum class ArrayFill
{
    None,
    Repeat,
    Extrapolate,
};

class InitList : public Syntax
{
public:
    ArrayFill Fill = ArrayFill::None;

    std::vector<Unique<Syntax>> Values;

    InitList();

    virtual void Accept( Visitor* visitor ) override;
};

class DeclSyntax : public Syntax
{
public:
    std::shared_ptr<Declaration> Decl;

    std::string Name;

    virtual Declaration* GetDecl() override;
};

class DataDecl : public DeclSyntax
{
public:
    Unique<Gemini::TypeRef>     TypeRef;
    Unique<Syntax>              Initializer;
};

class ConstDecl : public DataDecl
{
public:
    ConstDecl();

    virtual void Accept( Visitor* visitor ) override;
};

class VarDecl : public DataDecl
{
public:
    VarDecl();
    VarDecl( std::string_view name );

    virtual void Accept( Visitor* visitor ) override;
};

class ParamDecl : public DataDecl
{
public:
    virtual void Accept( Visitor* visitor ) override;
};

class TypeDecl : public DeclSyntax
{
public:
    Unique<Gemini::TypeRef>     TypeRef;

    virtual void Accept( Visitor* visitor ) override;
};

class LambdaExpr : public Syntax
{
public:
    Unique<ProcDecl> Proc;

    virtual void Accept( Visitor* visitor ) override;
};

class CondClause : public Syntax
{
public:
    Unique<Syntax> Condition;
    StatementList Body;

    virtual void Accept( Visitor* visitor ) override;
};

class CondExpr : public Syntax
{
public:
    std::vector<Unique<CondClause>> Clauses;
    bool IsIf = false;

    virtual void Accept( Visitor* visitor ) override;
};

class CaseWhen : public Syntax
{
public:
    std::vector<Unique<Syntax>> Keys;
    StatementList Body;

    virtual void Accept( Visitor* visitor ) override;
};

class CaseElse : public Syntax
{
public:
    StatementList Body;

    virtual void Accept( Visitor* visitor ) override;
};

class CaseExpr : public Syntax
{
public:
    Unique<Syntax> TestKey;
    Unique<CaseElse> Fallback;
    std::vector<Unique<CaseWhen>> Clauses;

    virtual void Accept( Visitor* visitor ) override;
};

class BinaryExpr : public Syntax
{
public:
    std::string Op;
    Unique<Syntax> Left;
    Unique<Syntax> Right;

    virtual void Accept( Visitor* visitor ) override;
};

class UnaryExpr : public Syntax
{
public:
    std::string Op;
    Unique<Syntax> Inner;

    virtual void Accept( Visitor* visitor ) override;
};

class AddrOfExpr : public Syntax
{
public:
    Unique<Syntax> Inner;

    AddrOfExpr();

    virtual void Accept( Visitor* visitor ) override;
};

class IndexExpr : public Syntax
{
public:
    Unique<Syntax> Head;
    Unique<Syntax> Index;

    IndexExpr();

    virtual void Accept( Visitor* visitor ) override;
};

class SliceExpr : public Syntax
{
public:
    Unique<Syntax> Head;
    Unique<Syntax> FirstIndex;
    Unique<Syntax> LastIndex;

    virtual void Accept( Visitor* visitor ) override;
};

class DotExpr : public Syntax
{
public:
    Unique<Syntax> Head;
    std::string Member;

    std::shared_ptr<Declaration> Decl;

    DotExpr();

    virtual void Accept( Visitor* visitor ) override;
    virtual Declaration* GetDecl() override;
};

class CallExpr : public Syntax
{
public:
    bool IsIndirect = false;
    Unique<Syntax> Head;
    std::vector<Unique<Syntax>> Arguments;

    virtual void Accept( Visitor* visitor ) override;
};

class CallOrSymbolExpr : public Syntax
{
public:
    Unique<Syntax> Symbol;

    virtual void Accept( Visitor* visitor ) override;
};

class AssignmentExpr : public Syntax
{
public:
    Unique<Syntax> Left;
    Unique<Syntax> Right;

    virtual void Accept( Visitor* visitor ) override;
};

class CountofExpr : public Syntax
{
public:
    Unique<Syntax>      Expr;

    virtual void Accept( Visitor* visitor ) override;
};

class LetStatement : public Syntax
{
public:
    std::vector<Unique<DataDecl>> Variables;
    StatementList Body;

    virtual void Accept( Visitor* visitor ) override;
};

class ReturnStatement : public Syntax
{
public:
    Unique<Syntax> Inner;

    virtual void Accept( Visitor* visitor ) override;
};

enum class ForComparison
{
    Above,
    Below,
    Downto,
    To,
};

class ForStatement : public Syntax
{
public:
    ForComparison Comparison = ForComparison::Above;
    Unique<DataDecl> Index;
    Unique<Syntax> First;
    Unique<Syntax> Last;
    Unique<Syntax> Step;
    StatementList Body;

    std::shared_ptr<Declaration> IndexDecl;

    virtual void Accept( Visitor* visitor ) override;
};

class LoopStatement : public Syntax
{
public:
    StatementList Body;
    Unique<Syntax> Condition;

    virtual void Accept( Visitor* visitor ) override;
};

class WhileStatement : public Syntax
{
public:
    Unique<Syntax> Condition;
    StatementList Body;

    virtual void Accept( Visitor* visitor ) override;
};

class BreakStatement : public Syntax
{
public:
    virtual void Accept( Visitor* visitor ) override;
};

class NextStatement : public Syntax
{
public:
    virtual void Accept( Visitor* visitor ) override;
};

class ProcDeclBase : public DeclSyntax
{
public:
    constexpr static int16_t MaxParams = ParamSizeMax;
    constexpr static int16_t MaxLocals = LocalSizeMax;

    std::vector<Unique<DataDecl>>   Params;
    Unique<TypeRef>                 ReturnTypeRef;
};

class ProcDecl : public ProcDeclBase
{
public:
    StatementList Body;

    virtual void Accept( Visitor* visitor ) override;
};

class NativeDecl : public ProcDeclBase
{
public:
    Unique<Syntax> OptionalId;

    virtual void Accept( Visitor* visitor ) override;
};

class ImportDecl : public DeclSyntax
{
public:
    std::string OriginalName;

    virtual void Accept( Visitor* visitor ) override;
};

class Unit : public Syntax
{
    // All nodes in the syntax tree rooted in this Unit refer to this string
    std::vector<char> mFileName;

public:
    std::vector<Unique<DeclSyntax>> DataDeclarations;
    std::vector<Unique<ProcDecl>> FuncDeclarations;

    Unit( const std::string& fileName );

    const char* GetUnitFileName();

    virtual void Accept( Visitor* visitor ) override;
};


std::optional<int32_t> GetFinalOptionalSyntaxValue( Syntax* node );

void CopyBaseSyntax( Syntax& dest, const Syntax& source );


//----------------------------------------------------------------------------
//  Visitors
//----------------------------------------------------------------------------

class Visitor
{
public:
    virtual ~Visitor() { }

    virtual void VisitAddrOfExpr( AddrOfExpr* addrOf );
    virtual void VisitArrayTypeRef( ArrayTypeRef* typeRef );
    virtual void VisitAssignmentExpr( AssignmentExpr* assignment );
    virtual void VisitBinaryExpr( BinaryExpr* binary );
    virtual void VisitBreakStatement( BreakStatement* breakStmt );
    virtual void VisitCallExpr( CallExpr* call );
    virtual void VisitCallOrSymbolExpr( CallOrSymbolExpr* callOrSymbol );
    virtual void VisitCaseExpr( CaseExpr* caseExpr );
    virtual void VisitCondExpr( CondExpr* condExpr );
    virtual void VisitConstDecl( ConstDecl* constDecl );
    virtual void VisitCountofExpr( CountofExpr* countofExpr );
    virtual void VisitDotExpr( DotExpr* dotExpr );
    virtual void VisitForStatement( ForStatement* forStmt );
    virtual void VisitImportDecl( ImportDecl* importDecl );
    virtual void VisitIndexExpr( IndexExpr* indexExpr );
    virtual void VisitInitList( InitList* initList );
    virtual void VisitLambdaExpr( LambdaExpr* lambdaExpr );
    virtual void VisitLetStatement( LetStatement* letStmt );
    virtual void VisitLoopStatement( LoopStatement* loopStmt );
    virtual void VisitNameExpr( NameExpr* nameExpr );
    virtual void VisitNameTypeRef( NameTypeRef* nameTypeRef );
    virtual void VisitNativeDecl( NativeDecl* nativeDecl );
    virtual void VisitNextStatement( NextStatement* nextStmt );
    virtual void VisitNumberExpr( NumberExpr* numberExpr );
    virtual void VisitParamDecl( ParamDecl* paramDecl );
    virtual void VisitPointerTypeRef( PointerTypeRef* pointerTypeRef );
    virtual void VisitProcDecl( ProcDecl* procDecl );
    virtual void VisitProcTypeRef( ProcTypeRef* procTypeRef );
    virtual void VisitReturnStatement( ReturnStatement* retStmt );
    virtual void VisitSliceExpr( SliceExpr* sliceExpr );
    virtual void VisitStatementList( StatementList* stmtmList );
    virtual void VisitTypeDecl( TypeDecl* typeDecl );
    virtual void VisitUnaryExpr( UnaryExpr* unary );
    virtual void VisitUnit( Unit* unit );
    virtual void VisitVarDecl( VarDecl* varDecl );
    virtual void VisitWhileStatement( WhileStatement* whileStmt );
};


//----------------------------------------------------------------------------
//  Declarations
//----------------------------------------------------------------------------

enum class DeclKind
{
    Undefined,
    Const,
    Global,
    Local,
    Param,
    Func,
    NativeFunc,
    Type,
    Module,
    LoadedAddress,
};

struct Declaration
{
    const DeclKind                  Kind;
    std::shared_ptr<Gemini::Type>   Type;

    virtual ~Declaration() { }

protected:
    Declaration( DeclKind kind );
};

using SymTable = std::map<std::string, std::shared_ptr<Declaration>>;

struct UndefinedDeclaration : public Declaration
{
    // It would be safer if here we kept a weak_ptr to a Syntax node.
    // But that would mean changing all Unique<Syntax> to Shared<Syntax>.

    Syntax*     Node = nullptr;

    UndefinedDeclaration();
};

enum class ValueKind
{
    Integer,
    Function,
};

struct Function;

class ValueVariant
{
    ValueKind mKind = ValueKind::Integer;

    union ValueUnion
    {
        int32_t Integer;
        std::shared_ptr<Function> Function;

        ~ValueUnion() {}
    } mValue = { 0 };

public:
    ValueKind GetKind() const
    {
        return mKind;
    }

    int32_t GetInteger() const
    {
        assert( mKind == ValueKind::Integer );
        return mValue.Integer;
    }

    std::shared_ptr<Function> GetFunction() const
    {
        assert( mKind == ValueKind::Function );
        return mValue.Function;
    }

    void SetInteger( int32_t value )
    {
        Destroy();

        mValue.Integer = value;
        mKind = ValueKind::Integer;
    }

    void SetFunction( std::shared_ptr<Function> function )
    {
        Destroy();

        new (&mValue.Function) std::shared_ptr<Function>( function );
        mKind = ValueKind::Function;
    }

    ~ValueVariant()
    {
        Destroy();
    }

private:
    void Destroy()
    {
        if ( mKind == ValueKind::Function )
        {
            mValue.Function.~shared_ptr<Function>();
            mKind = ValueKind::Integer;
        }
    }
};

struct Constant : public Declaration
{
    ValueVariant Value;

    Constant();
};

struct GlobalStorage : public Declaration
{
    GlobalSize  Offset = 0;
    ModSize     ModIndex = 0;

    GlobalStorage();
};

struct LocalStorage : public Declaration
{
    LocalSize   Offset = 0;

    LocalStorage();
};

struct ParamStorage : public Declaration
{
    ParamSize   Offset = 0;

    ParamStorage();
};

struct CallSite
{
    std::string FunctionName;
    int16_t     ExprDepth;
    uint8_t     ModIndex;
};

struct Function : public Declaration
{
    std::string Name;
    CodeSize    Address = UndefinedAddr;
    ModSize     ModIndex = 0;
    bool        IsLambda = false;

    LocalSize   LocalCount = 0;
    ParamSize   ParamCount = 0;
    LocalSize   ExprDepth = 0;

    uint32_t    CallDepth = 0;
    uint32_t    IndividualStackUsage = 0;
    uint32_t    TreeStackUsage = 0;

    bool        IsCalculating = false;
    bool        IsRecursive = false;
    bool        IsDepthKnown = false;
    bool        CallsIndirectly = false;

    std::list<CallSite> CalledFunctions;

    Function();
};

struct NativeFunction : public Declaration
{
    int32_t     Id = 0;

    NativeFunction();
};

struct TypeDeclaration : public Declaration
{
    std::shared_ptr<Gemini::Type> ReferentType;

    TypeDeclaration();
};

struct ModuleDeclaration : public Declaration
{
    std::string Name;
    SymTable    Table;
    int32_t     Index = 0;

    ModuleDeclaration();
};

struct LoadedAddressDeclaration : public Declaration
{
    LoadedAddressDeclaration();
};


//----------------------------------------------------------------------------
//  Types
//----------------------------------------------------------------------------

enum class TypeKind
{
    Type,
    Module,
    Xfer,
    Int,
    Array,
    Func,
    Pointer,
};

class Type
{
    TypeKind    mKind;

protected:
    Type( TypeKind kind );

public:
    virtual ~Type() { }

    TypeKind GetKind() const;
    virtual bool IsEqual( Type* other ) const;
    virtual bool IsAssignableFrom( Type* other ) const;
    virtual DataSize GetSize() const;
};

class TypeType : public Type
{
public:
    TypeType();
};

class ModuleType : public Type
{
public:
    ModuleType();
};

class XferType : public Type
{
public:
    XferType();

    virtual bool IsEqual( Type* other ) const override;
};

class IntType : public Type
{
public:
    IntType();

    virtual bool IsEqual( Type* other ) const override;
    virtual bool IsAssignableFrom( Type* other ) const override;
    virtual DataSize GetSize() const override;
};

class ArrayType : public Type
{
public:
    DataSize Count;
    std::shared_ptr<Type> ElemType;

    ArrayType( DataSize count, std::shared_ptr<Type> elemType );

    virtual bool IsEqual( Type* other ) const override;
    virtual bool IsAssignableFrom( Type* other ) const override;
    virtual DataSize GetSize() const override;
};

class FuncType : public Type
{
public:
    std::shared_ptr<Type>               ReturnType;
    std::vector<std::shared_ptr<Type>>  ParamTypes;

    FuncType( std::shared_ptr<Type> returnType );

    virtual bool IsEqual( Type* other ) const override;
};

class PointerType : public Type
{
public:
    std::shared_ptr<Type>   TargetType;

    PointerType( std::shared_ptr<Type> target );

    virtual bool IsEqual( Type* other ) const override;
    virtual DataSize GetSize() const override;
};

}

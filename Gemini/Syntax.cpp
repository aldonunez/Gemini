// Gemini Languages and Virtual Machine
// Copyright 2021 Aldo Jose Nunez
//
// Licensed under the Apache License, Version 2.0.
// See the LICENSE.txt file for details.

#include "pch.h"
#include "Syntax.h"
#include <algorithm>
#include <assert.h>


namespace Gemini
{

Declaration* Syntax::GetDecl()
{
    return nullptr;
}

NameExpr::NameExpr()
{
    Kind = SyntaxKind::Name;
}

NameExpr::NameExpr( const std::string& str ) :
    String( str )
{
    Kind = SyntaxKind::Name;
}

NameExpr::NameExpr( std::string&& str ) :
    String( str )
{
    Kind = SyntaxKind::Name;
}

Declaration* NameExpr::GetDecl()
{
    return Decl.get();
}

NumberExpr::NumberExpr() :
    NumberExpr( 0 )
{
}

NumberExpr::NumberExpr( int64_t value ) :
    Value( value )
{
    Kind = SyntaxKind::Number;
}

InitList::InitList()
{
    Kind = SyntaxKind::ArrayInitializer;
}

Declaration* DeclSyntax::GetDecl()
{
    return Decl.get();
}

ConstDecl::ConstDecl()
{
    Kind = SyntaxKind::ConstDecl;
}

VarDecl::VarDecl()
{
    Kind = SyntaxKind::VarDecl;
}

VarDecl::VarDecl( std::string_view name ) :
    VarDecl()
{
    Name = name;
}

AddrOfExpr::AddrOfExpr()
{
    Kind = SyntaxKind::AddrOfExpr;
}

IndexExpr::IndexExpr()
{
    Kind = SyntaxKind::Index;
}

SliceExpr::SliceExpr()
{
    Kind = SyntaxKind::Slice;
}

DotExpr::DotExpr()
{
    Kind = SyntaxKind::DotExpr;
}

Declaration* DotExpr::GetDecl()
{
    return Decl.get();
}

Unit::Unit( const std::string& fileName )
{
    mFileName.resize( fileName.size() + 1 );

    std::copy_n( fileName.data(), fileName.size(), mFileName.data() );
    mFileName[fileName.size()] = '\0';

    FileName = GetUnitFileName();
}

const char* Unit::GetUnitFileName()
{
    return mFileName.data();
}


void AddrOfExpr::Accept( Visitor* visitor )
{
    visitor->VisitAddrOfExpr( this );
}

void ArrayTypeRef::Accept( Visitor* visitor )
{
    visitor->VisitArrayTypeRef( this );
}

void AssignmentExpr::Accept( Visitor* visitor )
{
    visitor->VisitAssignmentExpr( this );
}

void BinaryExpr::Accept( Visitor* visitor )
{
    visitor->VisitBinaryExpr( this );
}

void BreakStatement::Accept( Visitor* visitor )
{
    visitor->VisitBreakStatement( this );
}

void CallExpr::Accept( Visitor* visitor )
{
    visitor->VisitCallExpr( this );
}

void CallOrSymbolExpr::Accept( Visitor* visitor )
{
    visitor->VisitCallOrSymbolExpr( this );
}

void CaseElse::Accept( Visitor* visitor )
{
    // There's no entry in Visitor for this node
    assert( false );
}

void CaseExpr::Accept( Visitor* visitor )
{
    visitor->VisitCaseExpr( this );
}

void CaseWhen::Accept( Visitor* visitor )
{
    // There's no entry in Visitor for this node
    assert( false );
}

void CondClause::Accept( Visitor* visitor )
{
    // There's no entry in Visitor for this node
    assert( false );
}

void CondExpr::Accept( Visitor* visitor )
{
    visitor->VisitCondExpr( this );
}

void ConstDecl::Accept( Visitor* visitor )
{
    visitor->VisitConstDecl( this );
}

void CountofExpr::Accept( Visitor* visitor )
{
    visitor->VisitCountofExpr( this );
}

void DotExpr::Accept( Visitor* visitor )
{
    visitor->VisitDotExpr( this );
}

void ForStatement::Accept( Visitor* visitor )
{
    visitor->VisitForStatement( this );
}

void ImportDecl::Accept( Visitor* visitor )
{
    visitor->VisitImportDecl( this );
}

void IndexExpr::Accept( Visitor* visitor )
{
    visitor->VisitIndexExpr( this );
}

void InitList::Accept( Visitor* visitor )
{
    visitor->VisitInitList( this );
}

void LambdaExpr::Accept( Visitor* visitor )
{
    visitor->VisitLambdaExpr( this );
}

void LetStatement::Accept( Visitor* visitor )
{
    visitor->VisitLetStatement( this );
}

void LoopStatement::Accept( Visitor* visitor )
{
    visitor->VisitLoopStatement( this );
}

void NameExpr::Accept( Visitor* visitor )
{
    visitor->VisitNameExpr( this );
}

void NameTypeRef::Accept( Visitor* visitor )
{
    visitor->VisitNameTypeRef( this );
}

void NextStatement::Accept( Visitor* visitor )
{
    visitor->VisitNextStatement( this );
}

void NativeDecl::Accept( Visitor* visitor )
{
    visitor->VisitNativeDecl( this );
}

void NumberExpr::Accept( Visitor* visitor )
{
    visitor->VisitNumberExpr( this );
}

void ParamDecl::Accept( Visitor* visitor )
{
    visitor->VisitParamDecl( this );
}

void PointerTypeRef::Accept( Visitor* visitor )
{
    visitor->VisitPointerTypeRef( this );
}

void ProcDecl::Accept( Visitor* visitor )
{
    visitor->VisitProcDecl( this );
}

void ProcTypeRef::Accept( Visitor* visitor )
{
    visitor->VisitProcTypeRef( this );
}

void ReturnStatement::Accept( Visitor* visitor )
{
    visitor->VisitReturnStatement( this );
}

void SliceExpr::Accept( Visitor* visitor )
{
    visitor->VisitSliceExpr( this );
}

void StatementList::Accept( Visitor* visitor )
{
    visitor->VisitStatementList( this );
}

void TypeDecl::Accept( Visitor* visitor )
{
    visitor->VisitTypeDecl( this );
}

void UnaryExpr::Accept( Visitor* visitor )
{
    visitor->VisitUnaryExpr( this );
}

void Unit::Accept( Visitor* visitor )
{
    visitor->VisitUnit( this );
}

void VarDecl::Accept( Visitor* visitor )
{
    visitor->VisitVarDecl( this );
}

void WhileStatement::Accept( Visitor* visitor )
{
    visitor->VisitWhileStatement( this );
}


std::optional<int32_t> GetFinalOptionalSyntaxValue( Syntax* node )
{
    if ( node->Kind == SyntaxKind::Number )
    {
        auto number = (NumberExpr*) node;

        assert( number->Value >= INT32_MIN && number->Value <= INT32_MAX );

        return (int32_t) number->Value;
    }

    return std::optional<int32_t>();
}

void CopyBaseSyntax( Syntax& dest, const Syntax& source )
{
    dest.Line = source.Line;
    dest.Column = source.Column;
    dest.FileName = source.FileName;
}


//----------------------------------------------------------------------------
//  Visitors
//----------------------------------------------------------------------------

void Visitor::VisitAddrOfExpr( AddrOfExpr* addrOf )
{
}

void Visitor::VisitArrayTypeRef( ArrayTypeRef* typeRef )
{
}

void Visitor::VisitAssignmentExpr( AssignmentExpr* assignment )
{
}

void Visitor::VisitBinaryExpr( BinaryExpr* binary )
{
}

void Visitor::VisitBreakStatement( BreakStatement* breakStmt )
{
}

void Visitor::VisitCallExpr( CallExpr* call )
{
}

void Visitor::VisitCallOrSymbolExpr( CallOrSymbolExpr* callOrSymbol )
{
}

void Visitor::VisitCaseExpr( CaseExpr* caseExpr )
{
}

void Visitor::VisitCondExpr( CondExpr* condExpr )
{
}

void Visitor::VisitConstDecl( ConstDecl* constDecl )
{
}

void Visitor::VisitCountofExpr( CountofExpr* countofExpr )
{
}

void Visitor::VisitDotExpr( DotExpr* dotExpr )
{
}

void Visitor::VisitForStatement( ForStatement* forStmt )
{
}

void Visitor::VisitImportDecl( ImportDecl* importDecl )
{
}

void Visitor::VisitIndexExpr( IndexExpr* indexExpr )
{
}

void Visitor::VisitInitList( InitList* initList )
{
}

void Visitor::VisitLambdaExpr( LambdaExpr* lambdaExpr )
{
}

void Visitor::VisitLetStatement( LetStatement* letStmt )
{
}

void Visitor::VisitLoopStatement( LoopStatement* loopStmt )
{
}

void Visitor::VisitNameExpr( NameExpr* nameExpr )
{
}

void Visitor::VisitNameTypeRef( NameTypeRef* nameTypeRef )
{
}

void Visitor::VisitNativeDecl( NativeDecl* nativeDecl )
{
}

void Visitor::VisitNextStatement( NextStatement* nextStmt )
{
}

void Visitor::VisitNumberExpr( NumberExpr* numberExpr )
{
}

void Visitor::VisitParamDecl( ParamDecl* paramDecl )
{
}

void Visitor::VisitPointerTypeRef( PointerTypeRef* pointerTypeRef )
{
}

void Visitor::VisitProcDecl( ProcDecl* procDecl )
{
}

void Visitor::VisitProcTypeRef( ProcTypeRef* procTypeRef )
{
}

void Visitor::VisitReturnStatement( ReturnStatement* retStmt )
{
}

void Visitor::VisitSliceExpr( SliceExpr* sliceExpr )
{
}

void Visitor::VisitStatementList( StatementList* stmtmList )
{
}

void Visitor::VisitTypeDecl( TypeDecl* typeDecl )
{
}

void Visitor::VisitUnaryExpr( UnaryExpr* unary )
{
}

void Visitor::VisitUnit( Unit* unit )
{
}

void Visitor::VisitVarDecl( VarDecl* varDecl )
{
}

void Visitor::VisitWhileStatement( WhileStatement* whileStmt )
{
}


//----------------------------------------------------------------------------
//  Declarations
//----------------------------------------------------------------------------

Declaration::Declaration( DeclKind kind ) :
    Kind( kind )
{
}

UndefinedDeclaration::UndefinedDeclaration() :
    Declaration( DeclKind::Undefined )
{
}

Constant::Constant() :
    Declaration( DeclKind::Const )
{
}

GlobalStorage::GlobalStorage() :
    Declaration( DeclKind::Global )
{
}

LocalStorage::LocalStorage() :
    Declaration( DeclKind::Local )
{
}

ParamStorage::ParamStorage() :
    Declaration( DeclKind::Param )
{
}

Function::Function() :
    Declaration( DeclKind::Func )
{
}

NativeFunction::NativeFunction() :
    Declaration( DeclKind::NativeFunc )
{
}

TypeDeclaration::TypeDeclaration() :
    Declaration( DeclKind::Type )
{
}

ModuleDeclaration::ModuleDeclaration() :
    Declaration( DeclKind::Module )
{
}

LoadedAddressDeclaration::LoadedAddressDeclaration() :
    Declaration( DeclKind::LoadedAddress )
{
}


//----------------------------------------------------------------------------
//  Types
//----------------------------------------------------------------------------

Type::Type( TypeKind kind ) :
    mKind( kind )
{
}

TypeKind Type::GetKind() const
{
    return mKind;
}

bool Type::IsEqual( Type* other ) const
{
    return false;
}

bool Type::IsAssignableFrom( Type* other ) const
{
    return IsEqual( other );
}

DataSize Type::GetSize() const
{
    return 0;
}


TypeType::TypeType() :
    Type( TypeKind::Type )
{
}


ModuleType::ModuleType() :
    Type( TypeKind::Module )
{
}


XferType::XferType() :
    Type( TypeKind::Xfer )
{
}

bool XferType::IsEqual( Type* other ) const
{
    return other != nullptr
        && other->GetKind() == TypeKind::Xfer;
}


IntType::IntType() :
    Type( TypeKind::Int )
{
}

bool IntType::IsEqual( Type* other ) const
{
    return other != nullptr
        && (other->GetKind() == TypeKind::Int);
}

bool IntType::IsAssignableFrom( Type* other ) const
{
    return other != nullptr
        && (other->GetKind() == TypeKind::Int
            || other->GetKind() == TypeKind::Xfer
            );
}

DataSize IntType::GetSize() const
{
    return 1;
}


ArrayType::ArrayType( DataSize count, std::shared_ptr<Type> elemType ) :
    Type( TypeKind::Array ),
    Count( count ),
    ElemType( elemType )
{
}

bool ArrayType::IsEqual( Type* other ) const
{
    if ( other == nullptr || other->GetKind() != TypeKind::Array )
        return false;

    auto otherArray = (ArrayType*) other;

    if ( !ElemType->IsEqual( otherArray->ElemType.get() ) )
        return false;

    return Count == otherArray->Count;
}

bool ArrayType::IsAssignableFrom( Type* other ) const
{
    if ( other == nullptr || other->GetKind() != TypeKind::Array )
        return false;

    auto otherArray = (ArrayType*) other;

    if ( !ElemType->IsEqual( otherArray->ElemType.get() ) )
        return false;

    if ( ElemType->GetKind() == TypeKind::Pointer )
        return Count == otherArray->Count;

    return Count >= otherArray->Count;
}

DataSize ArrayType::GetSize() const
{
    return Count * ElemType->GetSize();
}


FuncType::FuncType( std::shared_ptr<Type> returnType ) :
    Type( TypeKind::Func ),
    ReturnType( returnType )
{
}

bool FuncType::IsEqual( Type* other ) const
{
    if ( other == nullptr || other->GetKind() != TypeKind::Func )
        return false;

    auto otherFunc = (FuncType*) other;

    if ( !ReturnType->IsEqual( otherFunc->ReturnType.get() )
        || ParamTypes.size() != otherFunc->ParamTypes.size() )
        return false;

    for ( int i = 0; i < (int) ParamTypes.size(); i++ )
    {
        if ( !ParamTypes[i]->IsEqual( otherFunc->ParamTypes[i].get() ) )
            return false;
    }

    return true;
}


PointerType::PointerType( std::shared_ptr<Type> target ) :
    Type( TypeKind::Pointer ),
    TargetType( target )
{
}

bool PointerType::IsEqual( Type* other ) const
{
    if ( other == nullptr || other->GetKind() != TypeKind::Pointer )
        return false;

    auto otherPointer = (PointerType*) other;

    return TargetType->IsEqual( otherPointer->TargetType.get() );
}

DataSize PointerType::GetSize() const
{
    return 1;
}

}

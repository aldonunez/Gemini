#include "pch.h"
#include "TestBase.h"


//-----------------------------------------

//=============================================

#if 0
TEST_CASE( "Lispy: ???", "[lispy]" )
{
    // TODO: Trying to test "If without else must yield integers or nothing"

    const char code[] =
        "(defun a () (let ((m 0) (n 0)) (cond (n (function B))) ))"
        "(defun B () )"
        ;

    TestCompileAndRunLispy( code, sizeof code, 0 );
}
#endif

TEST_CASE( "Lispy: Negative number", "[lispy]" )
{
    const char code[] =
        "(defun a () (if 2 -3))"
        ;

    TestCompileAndRunLispy( code, sizeof code, -3 );
}

TEST_CASE( "Lispy: Cond, literal condition, no implicit progn", "[lispy]" )
{
    const char code[] =
        "(defun a () (cond (2) (3 (B))) )"
        "(defun B () )"
        ;

    TestCompileAndRunLispy( code, sizeof code, 2 );
}

TEST_CASE( "Lispy: Cond, literal condition fallback", "[lispy]" )
{
    const char code[] =
        "(defun a () (let ((x 0)) (cond ((= x 2)) (3 (B))) ))"
        "(defun B () 99)"
        ;

    TestCompileAndRunLispy( code, sizeof code, 99 );
}

TEST_CASE( "Lispy: Cond fallback to nil", "[lispy]" )
{
    const char code[] =
        "(defun a () (let ((x 0)) (cond ((= x 2)) ((= x 3) (B))) ))"
        "(defun B () 99)"
        ;

    TestCompileAndRunLispy( code, sizeof code, 0 );
}

TEST_CASE( "Lispy: If-Else, progn", "[lispy]" )
{
    const char code[] =
        "(defun a () (let ((x 0)) (if (= x 2) (B) (progn 3 (B))) ))"
        "(defun B () 99)"
        "(defun C () 77)"
        ;

    TestCompileAndRunLispy( code, sizeof code, 99 );
}

// FAIL:
//"(defun b () (let ((x (function b))) (case 1 (1 (function a)) (2 (function b)) ) ) 9)\n"
//"(defun a () (b) )"
//"(defun c () )"

TEST_CASE( "Lispy: Case-else, func result from 2 clauses and else", "[lispy][syntax]" )
{
    const char code[] =
        "(defun a (y) (funcall \n"
        "  (case y (1 (function B)) (2 (function C)) (otherwise (function D))) ))\n"
        "(defun B () 10 ) (defun C () 20 ) (defun D () 30 )"
        ;

    WHEN( "1" )
        TestCompileAndRunLispy( code, sizeof code, 10, 1 );

    WHEN( "2" )
        TestCompileAndRunLispy( code, sizeof code, 20, 2 );

    WHEN( "Else" )
        TestCompileAndRunLispy( code, sizeof code, 30, 3 );
}

TEST_CASE( "Lispy: Cond, 1 clause non-lit cond with int result", "[lispy]" )
{
    const char code[] =
        "(defun a (y) (let ((x 11)) (set x (cond ((= y 1) 2)) ) x))\n"
        ;

    WHEN( "5" )
        TestCompileAndRunLispy( code, sizeof code, 0, 5 );

    WHEN( "1" )
        TestCompileAndRunLispy( code, sizeof code, 2, 1 );
}

TEST_CASE( "Lispy: Cond-else, 1 clause non-lit cond with int result", "[lispy]" )
{
    const char code[] =
        "(defun a (y) (let ((x 11)) (set x (cond ((= y 1) 2) (true 3)) ) x))\n"
        ;

    WHEN( "5" )
        TestCompileAndRunLispy( code, sizeof code, 3, 5 );

    WHEN( "1" )
        TestCompileAndRunLispy( code, sizeof code, 2, 1 );
}

TEST_CASE( "Lispy: Cond-else, 1 clause non-lit cond with func result", "[lispy]" )
{
    const char code[] =
        "(defun a (y) (let ((x (function B)))\n"
        "  (set x (cond ((= y 1) (function C)) (true (function D))) ) (funcall x)\n"
        "))\n"
        "(defun B () 10)(defun C () 20)(defun D () 30)"
        ;

    WHEN( "5" )
        TestCompileAndRunLispy( code, sizeof code, 30, 5 );

    WHEN( "1" )
        TestCompileAndRunLispy( code, sizeof code, 20, 1 );
}

TEST_CASE( "Lispy: Cond, empty", "[lispy]" )
{
    const char code[] =
        "(defun a () (+ (cond) 1) )"
        ;

    TestCompileAndRunLispy( code, sizeof code, 1 );
}

TEST_CASE( "Lispy: not, unary minus, call, if", "[lispy]" )
{
    const char code[] =
        "(defun B (x) (if (= x 10) 3))\n"
        "(defun a (x) (not (- (B x))) )"
        ;

    WHEN( "0" )
        TestCompileAndRunLispy( code, sizeof code, 1, 0 );

    WHEN( "10" )
        TestCompileAndRunLispy( code, sizeof code, 0, 10 );
}

TEST_CASE( "Lispy: Call lambda directly", "[lispy]" )
{
    const char code[] =
        "(defun a () (funcall (lambda () 5)) )\n"
        ;

    TestCompileAndRunLispy( code, sizeof code, 5 );
}

TEST_CASE( "Lispy: For-below-by, literal bounds, break, set", "[lispy]" )
{
    const char code[] =
        "(defun a () (let ((p 0)) (loop for i from 0 below 10 by 5 do "
        "(set p (+ p 1))"
        "(break)"
        "(set p (+ p 1))"
        ") p))\n"
        ;

    TestCompileAndRunLispy( code, sizeof code, 1 );
}

TEST_CASE( "Lispy: While, set", "[lispy]" )
{
    const char code[] =
        "(defun a () (let ((p 0)) (do (< p 3) (set p (+ p 1))"
        ") p))\n"
        ;

    TestCompileAndRunLispy( code, sizeof code, 3 );
}

TEST_CASE( "Lispy: Case, int results", "[lispy]" )
{
    const char code[] =
        "(defun a (x) (case (+ x 1) (1 10) ((5 6) 99) ))"
        ;

    WHEN( "1" )
        TestCompileAndRunLispy( code, sizeof code, 10, 0 );

    WHEN( "5" )
        TestCompileAndRunLispy( code, sizeof code, 99, 4 );

    WHEN( "6" )
        TestCompileAndRunLispy( code, sizeof code, 99, 5 );

    WHEN( "7" )
        TestCompileAndRunLispy( code, sizeof code, 0, 7 );
}

TEST_CASE( "Lispy: Global ints", "[lispy]" )
{
    const char code[] =
        "(defvar b 2 )\n"
        "(defvar c 3 )\n"
        "(defvar d )\n"
        "(defun a ()\n"
        "  (+ (+ b c) d) )"
        ;

    TestCompileAndRunLispy( code, sizeof code, 5 );
}

TEST_CASE( "Lispy: Global array", "[lispy]" )
{
    const char code[] =
        "(defvar b 2 )\n"
        "(defvar c 3 )\n"
        "(defvar (d : array (3)) (array 10 20 30) )\n"
        "(defun a ()\n"
        "  (+ (+ b c) (aref d 1)) )"
        ;

    TestCompileAndRunLispy( code, sizeof code, 25 );
}

TEST_CASE( "Lispy: Global array implicit", "[lispy]" )
{
    const char code[] =
        "(defvar b 2 )\n"
        "(defvar c 3 )\n"
        "(defvar d (array 10 20 30) )\n"
        "(defun a ()\n"
        "  (+ (+ b c) (aref d 1)) )"
        ;

    TestCompileAndRunLispy( code, sizeof code, 25 );
}

TEST_CASE( "Lispy: Local array refs in binary exprs, init array with mixed calls and number", "[lispy]" )
{
    const char code[] =
        "(defun B (x) (+ x 1) )\n"
        "(defun a ()\n"
        " (let ( ((d : array (3)) (array (B 0) (/ 4 2) (B 2) )) )"
        "   (+ (aref d 0) (+ (aref d 1) (aref d 2)))"
        "   ))"
        ;

    TestCompileAndRunLispy( code, sizeof code, 6 );
}


//---------------------------------------------

TEST_CASE( "abc_a", "[algoly]" )
{
    const char code[] =
        "def a 33 end"
        ;

    TestCompileAndRunAlgoly( code, sizeof code, 33 );
}

TEST_CASE( "abc_l", "[lispy]" )
{
    const char code[] =
        "(defun a () 33 )"
        ;

    TestCompileAndRunLispy( code, sizeof code, 33 );
}


#if 0
    std::shared_ptr<ModuleDeclaration> modDecl( new ModuleDeclaration() );

    modDecl->Kind = DeclKind::Module;
    modDecl->Name = "ModA";
    modDecl->Type = std::make_shared<ModuleType>();

    auto intType = std::make_shared<IntType>();
    auto typeType = std::make_shared<TypeType>();

    auto ftype = std::make_shared<FuncType>( intType );
    ftype->ParamTypes.push_back( intType );

    auto ptype = std::make_shared<PointerType>( ftype );

    auto pfTypeDecl = std::make_shared<TypeDeclaration>();
    pfTypeDecl->Kind = DeclKind::Type;
    pfTypeDecl->Type = typeType;
    pfTypeDecl->ReferentType = ptype;

    modDecl->Table.insert( SymTable::value_type( "P", pfTypeDecl ) );

    AddModule( modDecl );
#endif

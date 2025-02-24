#include <tl/support/containers/Opt.h>
#include <tl/support/log/TestingLog.h>
#include <tl/parse/Ast/StringStore.h>
#include <tl/parse/TokFromTxt.h>
#include <tl/parse/PstFromTok.h>
#include "catch_main.h"

using namespace Pst;
using namespace Tok;

void test_pst( Str code ) {
    Str file = "command_line";
    Ast::StringStore sst;
    TestingLog log;

    TokFromTxt ts( log );
    ts.parse_txt( code, sst.string( "file" ) );

    PstFromTok pt( log );
    pt.parse( ts.root() );

    AstFromPst ap( log );
    ap.parse( pt.root() );

    Displayer ds;
    pt.root()->display( ds );
    P( ds.as_Str( DisplayParameters::compact() ) );
}

TEST_CASE( "Parser src to tok", "" ) {
    // auto call
    test_tok( "a b, c", { .exp = "(a,b,c)" } );
    test_tok( "a b c", { .exp = "(a,(b,c))" } );

    // () call
    test_tok( "a( b, c )", { .exp = "(a,b,c)" } );
    test_tok( "a( b, c ) d", { .exp = "((a,b,c),d)" } );
    test_tok( "a( b, c ) + d", { .exp = "(operator +,(a,b,c),d)" } );

    // basic operator precedance
    test_tok( "a + b * c", { .exp = "(operator +,a,(operator *,b,c))" } );
    test_tok( "a * b + c", { .exp = "(operator +,(operator *,a,b),c)" } );
    test_tok( "a + b c", { .exp = "(operator +,a,(b,c))" } );

    // new lines
    test_tok( "a\n" "  b\n" , { .exp = "(a,b)" } );
    test_tok( "a\n" "  b\n" "  c\n" , { .exp = "(a,b,c)" } );
    test_tok( "a\n" "  b,\n" "  c\n" , { .exp = "(a,b,c)" } );
    test_tok( "a\n" "  b, c\n" "  d, e\n" , { .exp = "(a,b,c,d,e)" } );
    test_tok( "a\n" "  b, c\n" "  d e, f\n" , { .exp = "(a,b,c,(d,e,f))" } );

    // ;
    test_tok( "a\n" "  b; c\n" , { .exp = "(a,b,c)" } );
    test_tok( "a\n" "  b c; d\n" , { .exp = "(a,(b,c),d)" } );
    test_tok( "a( b c; d )" , { .exp = "(a,(b,c),d)" } );
    test_tok( "a( b c, d )" , { .exp = "(a,(b,c,d))" } );

    // .
    test_tok( "a.b c" , { .exp = "((operator .,a,b),c)" } );

    // (), [], {}
    test_tok( "( a )" , { .exp = "(operator (),a)" } );
    test_tok( "( a, b )" , { .exp = "(operator (),a,b)" } );
    test_tok( "( a + b )" , { .exp = "(operator (),(operator +,a,b))" } );
    test_tok( "( a + )" , { .exp = "(operator (),(operator +,a))", .err_msgs = Vec<Str>{ "token was expecting an additional child." } } );
}

TEST_CASE( "Parser src to ap", "" ) {
    test_pst( "a := b" );
}

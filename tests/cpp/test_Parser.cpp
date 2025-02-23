#include <tl/support/containers/Opt.h>
#include <tl/support/log/TestingLog.h>
#include <tl/parse/TokFromSrc.h>
#include <tl/parse/PstFromTok.h>
#include <tl/parse/AstFromPst.h>
#include "catch_main.h"

using namespace Ast;
//using namespace Pst;
using namespace Tok;

struct TestResult {
    Opt<Vec<Str>> err_msgs;
    Opt<Vec<PI>> err_pos;
    Opt<Str> exp;
};

void test_tok( Str code, TestResult tr ) {
    Str file = "command_line";
    StringStore sst;
    TestingLog log;

    TokFromSrc tp( log );
    tp.parse( code, 0, sst.string( "file" ) );

    bool made_a_test = false;
    if ( tr.err_msgs ) {
        CHECK( log.messages.size() == tr.err_msgs->size() );
        for( PI i = 0; i < log.messages.size(); ++i )
            CHECK( log.messages[ i ].msg == tr.err_msgs->operator[]( i ) );
        made_a_test = true;
    }

    if ( tr.err_pos ) {
        CHECK( log.messages.size() == tr.err_pos->size() );
        for( PI i = 0; i < log.messages.size(); ++i )
            CHECK( log.messages[ i ].pr.offsets[ 0 ] == tr.err_pos->operator[]( i ) );
        made_a_test = true;
    }

    if ( ! made_a_test )
        CHECK( log.messages.empty() );

    if ( tr.exp ) {
        CHECK( tp.condensed() == *tr.exp );
        made_a_test = true;
    }

    if ( ! made_a_test )
        P( tp.condensed() );
}

void test_pst( Str code ) {
    Str file = "command_line";
    StringStore sst;
    TestingLog log;

    TokFromSrc ts( log );
    ts.parse( code, 0, sst.string( "file" ) );

    PstFromTok pt( log );
    pt.parse( ts.root() );

    AstFromPst ap( log );
    ap.parse( pt.root() );

    P( pt.crepr() );

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

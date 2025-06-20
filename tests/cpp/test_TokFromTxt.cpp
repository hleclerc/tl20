#include <tl/support/containers/Opt.h>
#include <tl/support/log/TestingLog.h>
#include <tl/parse/TokFromTxt.h>
#include "catch_main.h"

using namespace Tok;

struct TestResult {
    Opt<Vec<Str>> err_msgs;
    Opt<Vec<PI>> err_pos;
    Opt<Str> exp;
};

void test_tok( Str code, TestResult tr ) {
    Str file = "command_line";
    TestingLog log;

    TokFromTxt tp( log );
    tp.parse_txt( code, file );
    tp.parse_eof();

    bool made_a_test = false;
    if ( tr.err_msgs ) {
        CHECK( log.messages.size() == tr.err_msgs->size() );
        for( PI i = 0; i < log.messages.size(); ++i )
            CHECK( log.messages[ i ].msg == tr.err_msgs->operator[]( i ) );
    }
    
    if ( tr.err_pos ) {
        CHECK( log.messages.size() == tr.err_pos->size() );
        for( PI i = 0; i < log.messages.size(); ++i )
            CHECK( log.messages[ i ].pr.offsets[ 0 ] == tr.err_pos->operator[]( i ) );
        made_a_test = true;
    }

    if ( ! made_a_test ) {
        if ( ! log.messages.empty() )
            PE( log.messages[ 0 ].msg );
        CHECK( log.messages.empty() );
    }

    if ( tr.exp ) {
        CHECK( tp.root()->condensed() == *tr.exp );
        made_a_test = true;
    }

    if ( ! made_a_test )
        P( tp.root()->condensed() );
}

TEST_CASE( "Parser src to tok", "" ) {
    // variables
    test_tok( "a", { .exp = "a" } );
    test_tok( "yaourt", { .exp = "yaourt" } );
    test_tok( "ya ou, rt", { .exp = "(ya,ou,rt)" } );
    test_tok( "a120_2", { .exp = "a120_2" } );

    // variables
    test_tok( "102.4"    , { .exp = "(number,\"102.4\")" } );
    test_tok( "102.4+a"  , { .exp = "(operator +,(number,\"102.4\"),a)" } );
    test_tok( "102.4a"   , { .exp = "(number,\"102.4a\")" } );
    test_tok( "102.4e+10", { .exp = "(number,\"102.4e+10\")" } );
    test_tok( "102.4e-10", { .exp = "(number,\"102.4e-10\")" } );
    test_tok( "102.4+10" , { .exp = "(operator +,(number,\"102.4\"),(number,\"10\"))" } );
    test_tok( "102e+10+1", { .exp = "(operator +,(number,\"102e+10\"),(number,\"1\"))" } );
    test_tok( "102.4.5"  , { .exp = "(operator .,(number,\"102.4\"),(number,\"5\"))" } );

    // // auto call
    // test_tok( "a b, c", { .exp = "(a,b,c)" } );
    // test_tok( "a b c", { .exp = "(a,(b,c))" } );

    // // () call
    // test_tok( "a( b, c )", { .exp = "(a,b,c)" } );
    // test_tok( "a( b, c ) d", { .exp = "((a,b,c),d)" } );
    // test_tok( "a( b, c ) + d", { .exp = "(operator +,(a,b,c),d)" } );

    // // basic operator precedance
    // test_tok( "a + b * c", { .exp = "(operator +,a,(operator *,b,c))" } );
    // test_tok( "a * b + c", { .exp = "(operator +,(operator *,a,b),c)" } );
    // test_tok( "a + b c", { .exp = "(operator +,a,(b,c))" } );

    // // new lines
    // test_tok( "a\n" "  b\n" , { .exp = "(a,b)" } );
    // test_tok( "a\n" "  b\n" "  c\n" , { .exp = "(a,b,c)" } );
    // test_tok( "a\n" "  b,\n" "  c\n" , { .exp = "(a,b,c)" } );
    // test_tok( "a\n" "  b, c\n" "  d, e\n" , { .exp = "(a,b,c,d,e)" } );
    // test_tok( "a\n" "  b, c\n" "  d e, f\n" , { .exp = "(a,b,c,(d,e,f))" } );

    // // ;
    // test_tok( "a\n" "  b; c\n" , { .exp = "(a,b,c)" } );
    // test_tok( "a\n" "  b c; d\n" , { .exp = "(a,(b,c),d)" } );
    // test_tok( "a( b c; d )" , { .exp = "(a,(b,c),d)" } );
    // test_tok( "a( b c, d )" , { .exp = "(a,(b,c,d))" } );

    // // .
    // test_tok( "a.b c" , { .exp = "((operator .,a,b),c)" } );

    // // (), [], {}
    // test_tok( "( a )" , { .exp = "(operator (),a)" } );
    // test_tok( "( a, b )" , { .exp = "(operator (),a,b)" } );
    // test_tok( "( a + b )" , { .exp = "(operator (),(operator +,a,b))" } );
    // test_tok( "( a + )" , { .exp = "(operator (),(operator +,a))", .err_msgs = Vec<Str>{ "token was expecting an additional child." } } );
}

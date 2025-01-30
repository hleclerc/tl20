#include <tl/support/containers/Opt.h>
#include <tl/support/log/TestingLog.h>
#include <tl/parse/AstWriterStr.h>
#include <tl/parse/TlParser.h>
#include "catch_main.h"

struct TestResult {
    Opt<Vec<Str>> err_msgs;
    Opt<Vec<PI>> err_pos;
    Opt<Str> exp;
};

void test( Str code, TestResult tr ) {
    Str file = "command_line";

    TestingLog log;
    AstWriter aw;
    TlParser tp( log );
    tp.parse( code, 0, aw.str( "file" ) );

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

TEST_CASE( "Parser", "" ) {
    // auto call
    test( "a b, c", { .exp = "(a,b,c)" } );
    test( "a b c", { .exp = "(a,(b,c))" } );

    // () call
    test( "a( b, c )", { .exp = "(a,b,c)" } );
    test( "a( b, c ) d", { .exp = "((a,b,c),d)" } );
    test( "a( b, c ) + d", { .exp = "(operator +,(a,b,c),d)" } );

    // basic operator precedance
    test( "a + b * c", { .exp = "(operator +,a,(operator *,b,c))" } );
    test( "a * b + c", { .exp = "(operator +,(operator *,a,b),c)" } );
    test( "a + b c", { .exp = "(operator +,a,(b,c))" } );

    // new lines
    test( "a\n" "  b\n" , { .exp = "(a,b)" } );
    test( "a\n" "  b\n" "  c\n" , { .exp = "(a,b,c)" } );
    test( "a\n" "  b,\n" "  c\n" , { .exp = "(a,b,c)" } );
    test( "a\n" "  b, c\n" "  d, e\n" , { .exp = "(a,b,c,d,e)" } );
    test( "a\n" "  b, c\n" "  d e, f\n" , { .exp = "(a,b,c,(d,e,f))" } );

    // ;
    test( "a\n" "  b; c\n" , { .exp = "(a,b,c)" } );
    test( "a\n" "  b c; d\n" , { .exp = "(a,(b,c),d)" } );
    test( "a( b c; d )" , { .exp = "(a,(b,c),d)" } );
    test( "a( b c, d )" , { .exp = "(a,(b,c,d))" } );

    // .
    test( "a.b c" , { .exp = "((operator .,a,b),c)" } );

    // (), [], {}
    test( "( a )" , { .exp = "(operator (),a)" } );
    test( "( a, b )" , { .exp = "(operator (),a,b)" } );
    test( "( a + b )" , { .exp = "(operator (),(operator +,a,b))" } );
    test( "( a + )" , { .exp = "(operator (),(operator +,a))", .err_msgs = Vec<Str>{ "token was expecting an additional child." } } );
}

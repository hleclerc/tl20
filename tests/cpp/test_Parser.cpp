#include "tl/parse/AstWriterStr.h"
#include <tl/parse/TlParser.h>
#include "catch_main.h"

struct TestResult {
    Str exp;
};

void test( Str code, TestResult tr ) {
    Str file = "command_line";

    AstWriter aw;
    TlParser tp;
    tp.parse( code, 0, aw.str( "file" ) );

    bool made_a_test = false;
    if ( tr.exp.size() ) {
        CHECK( tp.condensed() == tr.exp );
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

    test( "( a )" , { .exp = "(operator (),a)" } );
    test( "( a, b )" , { .exp = "(operator (),a,b)" } );
    test( "( a + b )" , { .exp = "(operator (),(operator +,a,b))" } );
    test( "( a + )" , { .exp = "(operator (),(operator +,a))" } );
}

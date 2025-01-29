#include "tl/parse/AstWriterStr.h"
#include <tl/parse/TlParser.h>
#include "catch_main.h"

void test( Str code ) {
    Str file = "command_line";
}

TEST_CASE( "Parser", "" ) {
    AstWriter aw;

    TlParser tp;
    tp.parse( "a + b * c", 0, aw.str( "file" ) );
    P( tp );

    tp.dump( aw );
}

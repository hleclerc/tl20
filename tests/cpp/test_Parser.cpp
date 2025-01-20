#include <tl/parse/TlParser.h>
#include "catch_main.h"
#include "tl/parse/AstWriterStr.h"

TEST_CASE( "Parser", "" ) {
    AstWriter aw;

    TlParser tp;
    tp.parse( "StrView + content 10\n  a\n  b", 0, aw.str( "file" ) );
    P( tp );

    tp.dump( aw );
}

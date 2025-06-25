#include <tl/support/containers/Opt.h>
#include <tl/support/log/TestingLog.h>
#include <tl/parse/TokFromTxt.h>
#include <tl/parse/PstFromTok.h>
#include <tl/parse/Ast/Ast.h>
#include <type_traits>
#include "catch_main.h"

using namespace Pst;
using namespace Tok;


void test_pst( Str code ) {
    Str file = "command_line";
    TestingLog log;
    Ast ast;

    TokFromTxt ts( log );
    ts.parse_txt( code, ast.new_AString( file ) );

    PstFromTok pt( log );
    pt.parse( ts.root() );

    // P( ts.root()->condensed( 1 ) );
    // AstFromPst ap( log );
    // ap.parse( pt.root() );

    // P( pt.root() );
    Displayer ds;
    pt.root()->display( ds );
    P( ds.as_Str( DisplayParameters::for_debug_info() ) );
}

TEST_CASE( "Parser src to ap", "" ) {
    test_pst( "a := b" );
}

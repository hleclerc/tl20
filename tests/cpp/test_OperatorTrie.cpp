#include <tl/support/containers/Vec.h>
#include <tl/parse/OperatorTrie.h>
#include "catch_main.h"

Vec<Str> tokenize_symbol_op( OperatorTrie &ot, StrView str ) {
    Vec<Str> res;
    while ( OperatorTrie::OperatorData *od = ot.symbol_op( str ) ) {
        str.remove_prefix( od->str.size() );
        res << od->str;
    }
    return res;
}

TEST_CASE( "Operator trie", "" ) {
    OperatorTrie ot;
    P( tokenize_symbol_op( ot, "++*+" ) );
    P( ot.letter_op( "and" ) );
    P( ot.letter_op( "yo" ) );
}

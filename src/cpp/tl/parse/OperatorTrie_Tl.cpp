#include "OperatorTrie_Tl.h"
#include <limits>

BEG_TL_NAMESPACE

OperatorTrie_Tl *OperatorTrie_Tl::default_tl_operator_trie() {
    static OperatorTrie_Tl res;
    return &res;
}

OperatorTrie_Tl::OperatorTrie_Tl() {
    init_default_operators();
}

void OperatorTrie_Tl::init_default_operators() {
    const int I = std::numeric_limits<int>::max();
    int curr_prio = 0;

    auto decl = [&]( Str n, int take_left, bool l2r, int rch ) {
        auto *od = new OperatorData{ 
            .str = n, 
            .name = "operator " + n,
            .priority = curr_prio, 
            .take_left = take_left, 
            .l2r = l2r, 
            .min_rch = rch == I ? 0 : rch,
            .max_rch = rch,
        };
        if ( ( n[ 0 ] >= 'a' &&  n[ 0 ] <= 'z' ) || ( n[ 0 ] >= 'A' &&  n[ 0 ] <= 'Z' ) || n[ 0 ] == '_' )
            register_letter_operator( od );
        else
            register_symbol_operator( od );
    };

    auto std_sop = [&]( Str n, int take_left, bool l2r, int rch ) {
    };
    
    const int M = 2; // Mandatory
    const int O = 1; // Optional

    prio_assign = ++curr_prio;
    decl( ":="        , /*take_left*/ M, /*l2r*/ 0, /*rch*/ 1 );
    decl( "::"        , /*take_left*/ M, /*l2r*/ 0, /*rch*/ 1 );

    prio_lambda = ++curr_prio;
    decl( "=>"        , /*take_left*/ M, /*l2r*/ 1, /*rch*/ I );

    prio_argpar = ++curr_prio;
    decl( "pertinence", /*take_left*/ M, /*l2r*/ 1, /*rch*/ 1 );
    decl( "when"      , /*take_left*/ M, /*l2r*/ 0, /*rch*/ 1 );

    prio_argval = ++curr_prio;
    decl( "="         , /*take_left*/ M, /*l2r*/ 0, /*rch*/ 1 );
    decl( ":"         , /*take_left*/ M, /*l2r*/ 0, /*rch*/ 1 );

    prio_argval = ++curr_prio;
    decl( "=="        , /*take_left*/ M, /*l2r*/ 1, /*rch*/ 1 );
    decl( "<="        , /*take_left*/ M, /*l2r*/ 1, /*rch*/ 1 );
    decl( ">="        , /*take_left*/ M, /*l2r*/ 1, /*rch*/ 1 );
    decl( "!="        , /*take_left*/ M, /*l2r*/ 1, /*rch*/ 1 );
    decl( "<"         , /*take_left*/ M, /*l2r*/ 1, /*rch*/ 1 );
    decl( ">"         , /*take_left*/ M, /*l2r*/ 1, /*rch*/ 1 );

    prio_and = ++curr_prio;
    decl( "and"       , /*take_left*/ M, /*l2r*/ 1, /*rch*/ 1 );
    decl( "or"        , /*take_left*/ M, /*l2r*/ 1, /*rch*/ 1 );

    prio_add = ++curr_prio;
    decl( "++"        , /*take_left*/ M, /*l2r*/ 1, /*rch*/ 0 );
    decl( "+"         , /*take_left*/ O, /*l2r*/ 1, /*rch*/ 1 );
    decl( "-"         , /*take_left*/ O, /*l2r*/ 1, /*rch*/ 1 );

    prio_mul = ++curr_prio;
    decl( "*"         , /*take_left*/ M, /*l2r*/ 1, /*rch*/ 1 );
    decl( "/"         , /*take_left*/ M, /*l2r*/ 1, /*rch*/ 1 );

    prio_pow = ++curr_prio;
    decl( "**"        , /*take_left*/ M, /*l2r*/ 1, /*rch*/ 1 );

    prio_call = ++curr_prio;

    prio_dot = ++curr_prio;
    decl( "."         , /*take_left*/ M, /*l2r*/ 1, /*rch*/ 1 );
    decl( "->"        , /*take_left*/ M, /*l2r*/ 1, /*rch*/ 1 );
}

END_TL_NAMESPACE

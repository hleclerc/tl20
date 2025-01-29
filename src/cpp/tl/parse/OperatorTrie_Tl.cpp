#include "OperatorTrie_Tl.h"

BEG_TL_NAMESPACE

OperatorTrie_Tl *OperatorTrie_Tl::default_tl_operator_trie() {
    static OperatorTrie_Tl res;
    return &res;
}

OperatorTrie_Tl::OperatorTrie_Tl() {
    init_default_operators();
}

void OperatorTrie_Tl::init_default_operators() {
    int curr_prio = 0;

    prio_add = ++curr_prio;
    register_symbol_operator( new OperatorData{ .str = "++" , .name = "operator ++" , .priority = curr_prio, .take_left = 0, .l2r = true, .max_rch = 0 } );
    register_symbol_operator( new OperatorData{ .str = "+"  , .name = "operator +"  , .priority = curr_prio, .take_left = 1, .l2r = true, .max_rch = 1 } );

    prio_mul = ++curr_prio;
    register_symbol_operator( new OperatorData{ .str = "*"  , .name = "operator *"  , .priority = curr_prio, .take_left = 2, .l2r = true, .max_rch = 1 } );

    prio_and = ++curr_prio;
    register_letter_operator( new OperatorData{ .str = "and", .name = "operator and", .priority = curr_prio, .take_left = 2, .l2r = true, .max_rch = 1 } );
    register_letter_operator( new OperatorData{ .str = "or" , .name = "operator or" , .priority = curr_prio, .take_left = 2, .l2r = true, .max_rch = 1 } );

    prio_call = ++curr_prio;
}

END_TL_NAMESPACE

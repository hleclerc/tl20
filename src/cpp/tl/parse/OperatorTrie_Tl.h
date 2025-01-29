#pragma once

#include "OperatorTrie.h"

BEG_TL_NAMESPACE

/** 

*/
class OperatorTrie_Tl : public OperatorTrie {
public:
    static auto   default_tl_operator_trie() -> OperatorTrie_Tl *;

    /**/          OperatorTrie_Tl         ();

    void          init_default_operators  ();

    int           prio_add;
    int           prio_mul;
    int           prio_and;

    int           prio_call;
};

END_TL_NAMESPACE

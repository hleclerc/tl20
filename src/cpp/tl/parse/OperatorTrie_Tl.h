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

    int           prio_assign;
    int           prio_lambda;
    int           prio_argval;
    int           prio_argpar;
    int           prio_and;
    int           prio_add;
    int           prio_mul;
    int           prio_pow;
    int           prio_call;
    int           prio_dot;
};

END_TL_NAMESPACE

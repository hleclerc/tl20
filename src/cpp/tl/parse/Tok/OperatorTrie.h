#pragma once

#include "../../support/Displayer.h"
#include <map>

BEG_TL_NAMESPACE
namespace Tok {

/** 

*/
class OperatorTrie {
public:
    struct        OperatorData            { void display( Displayer &dp ) const; Str str, name; int priority, take_left = 0; bool l2r = true; int min_rch = 1, max_rch = 1; };
    using         LetterMap               = std::map<Str,OperatorData *>;
    struct        CharMap                 { ~CharMap(); CharMap *prev, *next[ 256 ]; OperatorData *operator_data; };
            
    /**/          OperatorTrie            ();
    /**/         ~OperatorTrie            ();

    OperatorData* symbol_op               ( StrView str ); ///< may take only a subpart of str (cone have to check operator_data->str.size())
    OperatorData* letter_op               ( StrView str ); ///< 
  
    void          register_letter_operator( OperatorData *operator_data );
    void          register_symbol_operator( OperatorData *operator_data );
 
    LetterMap     letter_map;
    CharMap*      symbol_map;

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

OperatorTrie *base_operator_trie();

} // namespace Tok
END_TL_NAMESPACE

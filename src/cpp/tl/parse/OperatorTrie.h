#pragma once

#include "../support/Displayer.h"
#include <map>

BEG_TL_NAMESPACE

/** 

*/
class OperatorTrie {
public:
    struct        OperatorData            { void display( Displayer &ds ) const; Str str, name; int priority; bool l2r = true; int max_rch = 1; };
    using         LetterMap               = std::map<Str,OperatorData *>;
    struct        CharMap                 { ~CharMap(); CharMap *prev, *next[ 256 ]; OperatorData *operator_data; };
            
    /**/          OperatorTrie            ();
    /**/         ~OperatorTrie            ();
       
    OperatorData* symbol_op               ( StrView str ); ///< may take only a subpart of str (cone have to check operator_data->str.size())
    OperatorData* letter_op               ( StrView str ); ///< 
  
    void          register_letter_operator( OperatorData *operator_data );
    void          register_symbol_operator( OperatorData *operator_data );
    void          init_default_operators  ();
 
    LetterMap     letter_map;
    CharMap*      symbol_map;
};

END_TL_NAMESPACE

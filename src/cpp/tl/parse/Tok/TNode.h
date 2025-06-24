#pragma once

#include "../../support/string/PString.h"
#include "../../support/string/GString.h"
#include "../../support/Displayer.h"
#include "../Ast/AString.h"

BEG_TL_NAMESPACE
namespace Tok {

/** 
*/
class TNode {
public:
    enum class      Type            { ParenthesisCall, BracketCall, BraceCall, Variable, String };
   
    void            repl_in_graph_by( TNode *token );
    void            add_child       ( TNode *child );
    Str             condensed       ( bool only_the_args = false ) const;
    void            display         ( Displayer &ds ) const;
    
    AString         beg_src_url;    ///< url of the source containing the first char
    PI              beg_src_off;    ///< offset of the first char in the source
    
    PString         string_content; ///<
    GString         variable_ref;   ///<
    Type            type;           ///<
       
    TNode*          first_child     = nullptr;
    TNode*          last_child      = nullptr;
    TNode*          parent          = nullptr;
    TNode*          prev            = nullptr;
    TNode*          next            = nullptr;
};

} // namespace Tok
END_TL_NAMESPACE

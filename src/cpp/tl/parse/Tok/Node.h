#pragma once

#include "../../support/memory/PoolStr.h"
#include "../../support/string/StrRef.h"
#include "../../support/Displayer.h"

BEG_TL_NAMESPACE
namespace Tok {

/** 
*/
class Node {
public:
    enum class      Type            { ParenthesisCall, BracketCall, BraceCall, Variable, String, Root };
   
    void            repl_in_graph_by( Node *token );
    void            add_child       ( Node *child );
    Str             condensed       () const;
    void            display         ( Displayer &ds ) const;
    
    StrRef          beg_src_url;    ///<
    PI              beg_src_off;    ///<
    Type            type;           ///<

    PoolStr         string_content; ///<
    StrRef          variable_ref;   ///<
       
    Node*           first_child     = nullptr;
    Node*           last_child      = nullptr;
    Node*           parent          = nullptr;
    Node*           prev            = nullptr;
    Node*           next            = nullptr;
};

} // namespace Tok
END_TL_NAMESPACE

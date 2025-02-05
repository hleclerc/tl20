#pragma once

#include "../../support/memory/PoolVec.h"
#include "../../support/memory/PoolStr.h"
#include "../../support/Displayer.h"
#include "../Ast/WriterString.h"

BEG_TL_NAMESPACE
namespace Tok {

/** 
*/
class Node {
public:
    enum class      Type            { ParenthesisCall, BracketCall, BraceCall, Variable, String };
    struct          SrcRef          { Ast::WriterString url; PI beg, end; };
   
    void            repl_in_graph_by( Node *token );
    void            add_child       ( Node *child );
    Str             condensed       () const;
    void            display         ( Displayer &ds ) const;
    
    PoolVec<SrcRef> src_refs;    
    PoolStr         content;    
    Type            type;    
       
    Node*          first_child     = nullptr;
    Node*          last_child      = nullptr;
    Node*          parent          = nullptr;
    Node*          prev            = nullptr;
    Node*          next            = nullptr;

    PI              scope_size_decl = 0; ///< scope size during declaration
};

} // namespace Tok
END_TL_NAMESPACE

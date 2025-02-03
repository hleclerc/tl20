#pragma once

#include "../support/memory/PoolVec.h"
#include "../support/memory/PoolStr.h"
#include "../support/Displayer.h"
#include "AstWriterStr.h"

BEG_TL_NAMESPACE
class TlTokenScope;

/** 
*/
class TlToken {
public:
    enum class      Type            { ParenthesisCall, BracketCall, BraceCall, Variable, String };
    struct          SrcRef          { AstWriterStr url; PI beg, end; };
   
    void            repl_in_graph_by( TlToken *token );
    void            add_child       ( TlToken *child );
    Str             condensed       () const;
    void            display         ( Displayer &ds ) const;
    
    PoolVec<SrcRef> src_refs;    
    PoolStr         content;    
    Type            type;    
       
    TlToken*        first_child     = nullptr;
    TlToken*        last_child      = nullptr;
    TlToken*        parent          = nullptr;
    TlToken*        prev            = nullptr;
    TlToken*        next            = nullptr;

    PI              scope_size_decl = 0; ///< scope size during declaration
    TlTokenScope*   scope           = nullptr;
};

END_TL_NAMESPACE

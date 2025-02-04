#pragma once

#include "Scope.h"
#include "Node.h"

BEG_TL_NAMESPACE
namespace PreAst {

/** 
    node of a "pre ast" graph, used to make a final Ast for tokens
*/
class VarDecl {
public:
    /**/            VarDecl( Scope *parent_scope );

    void            display( Displayer &ds ) const;
    void            write  ( AstWriter &aw ) const;

    PI              pos_in_parent_scope; ///<
    bool            is_a_func;           ///<
    Scope           scope;               ///< scope for the arguments (if function)
    TlToken*        token; 
};

} // namespace PreAst
END_TL_NAMESPACE

#pragma once

#include "PNode.h"

BEG_TL_NAMESPACE
namespace Pst {
class Scope;

/** 
    node of a "pre ast" graph, used to make a final Ast for tokens
*/
class Node_Variable : public PNode {
public:
    /**/            Node_Variable ( Tok::TNode *token, Scope *scope, GString name );

    virtual void    display       ( Displayer &dp ) const;
    virtual void    write         ( Ast &ast ) const;

    PI              nb_variables_in_scope_during_construction; ///< 
    Scope*          scope;        ///<
    GString         name;         ///<

    PI              num_in_scope; ///< set in second phase of parsing
};

} // namespace Pst
END_TL_NAMESPACE

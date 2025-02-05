#pragma once

#include "Node.h"

BEG_TL_NAMESPACE
namespace Pst {
class Scope;

/** 
    node of a "pre ast" graph, used to make a final Ast for tokens
*/
class Node_Variable : public Node {
public:
    /**/            Node_Variable ( Tok::Node *token, Scope *scope, const Str &name );

    virtual void    display       ( Displayer &ds ) const;
    virtual void    write         ( Ast::Writer &aw ) const;

    PI              nb_variables_in_scope_during_construction; ///< 
    Scope*          scope;        ///<
    Str             name;         ///<

    PI              num_in_scope; ///< set in second phase of parsing
};

} // namespace Pst
END_TL_NAMESPACE

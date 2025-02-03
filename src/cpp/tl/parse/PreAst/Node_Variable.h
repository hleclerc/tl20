#pragma once

#include "Node.h"

BEG_TL_NAMESPACE
class TlToken;
namespace PreAst {
class Scope;

/** 
    node of a "pre ast" graph, used to make a final Ast for tokens
*/
class Node_Variable : public Node {
public:
    /**/            Node_Variable( TlToken *token, Scope *scope, const Str &name );

    virtual void    display      ( Displayer &ds ) const;
    virtual void    write        ( AstWriter &aw ) const;

    PI              nb_variables_in_scope; ///< when variable is used
    Scope*          scope;       ///<
    Str             name;        ///< 
};

} // namespace PreAst
END_TL_NAMESPACE

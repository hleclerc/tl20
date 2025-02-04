#pragma once

#include "Scope.h"
#include "Node.h"
#include "Arg.h"

BEG_TL_NAMESPACE
namespace PreAst {

/** 
    node of a "pre ast" graph, used to make a final Ast for tokens
*/
class Node_Call : public Node {
public:
    /**/            Node_Call    ( TlToken *token, Scope *parent_scope );

    virtual void    display      ( Displayer &ds ) const;
    virtual void    write        ( AstWriter &aw ) const;

    Scope           scope;       ///<
    Node*           func;        ///<
    Vec<Arg>        args;        ///<
};

} // namespace PreAst
END_TL_NAMESPACE

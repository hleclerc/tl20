#pragma once

#include "Scope.h"
#include "Node.h"
#include "Arg.h"

BEG_TL_NAMESPACE
namespace Pst {

/** 
    node of a "pre ast" graph, used to make a final Ast for tokens
*/
class Node_Call : public Node {
public:
    /**/            Node_Call    ( Tok::Node *token, Scope *parent_scope );

    virtual void    display      ( Displayer &ds ) const;
    virtual void    write        ( Ast::Writer &aw ) const;

    Scope           scope;       ///<
    Node*           func;        ///<
    Vec<Arg>        args;        ///<
};

} // namespace Pst
END_TL_NAMESPACE

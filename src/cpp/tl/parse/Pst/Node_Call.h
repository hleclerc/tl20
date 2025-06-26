#pragma once

#include "Scope.h"
#include "Arg.h"

BEG_TL_NAMESPACE
namespace Pst {

/** 
    node of a "pre ast" graph, used to make a final Ast for tokens
*/
class Node_Call : public PNode {
public:
    /**/            Node_Call    ( Tok::TNode *token, Scope *parent_scope );

    virtual void    display      ( Displayer &dp ) const;
    virtual void    write        ( Ast &ast ) const;

    Scope           scope;       ///<
    PNode*          func;        ///<
    Vec<Arg>        args;        ///<
};

} // namespace Pst
END_TL_NAMESPACE

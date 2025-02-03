#pragma once

#include "Scope.h"
#include "Node.h"

BEG_TL_NAMESPACE
namespace PreAst {

/** 
    node of a "pre ast" graph, used to make a final Ast for tokens
*/
class Block : public Node {
public:
    /**/          Block    ( TlToken *token, Scope *parent_scope = nullptr, Scope::Type scope_type = Scope::Type::Immediate );

    virtual void  display  ( Displayer &ds ) const;
    virtual void  write    ( AstWriter &aw ) const;

    Scope         scope;
    Vec<Node *>   nodes;
};

} // namespace PreAst
END_TL_NAMESPACE

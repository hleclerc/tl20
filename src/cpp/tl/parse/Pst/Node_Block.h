#pragma once

#include "Block.h"

BEG_TL_NAMESPACE
namespace Pst {

/** 
    node of a "pre ast" graph, used to make a final Ast for tokens
*/
class Node_Block : public Node {
public:
    /**/          Node_Block( Tok::Node *token, Scope *parent_scope = nullptr, Scope::Type scope_type = Scope::Type::Immediate );

    virtual void  display   ( Displayer &ds ) const;
    virtual void  write     ( Ast::Writer &aw ) const;

    Block         block;
};

} // namespace Pst
END_TL_NAMESPACE

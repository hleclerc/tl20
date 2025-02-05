#pragma once

#include "Scope.h"
#include "Node.h"

BEG_TL_NAMESPACE
namespace Pst {

/** 
*/
class Block {
public:
    /* */         Block    ( Scope *parent_scope = nullptr, Scope::Type scope_type = Scope::Type::Immediate );
    
    void          display  ( Displayer &ds ) const;
    void          write    ( Ast::Writer &aw ) const;

    Scope         scope;
    Vec<Node *>   nodes;
};

} // namespace Pst
END_TL_NAMESPACE

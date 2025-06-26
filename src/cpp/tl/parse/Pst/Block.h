#pragma once

#include "Scope.h"
#include "PNode.h"

BEG_TL_NAMESPACE
namespace Pst {

/** 
*/
class Block {
public:
    /* */         Block    ( Scope *parent_scope = nullptr, Scope::Type scope_type = Scope::Type::Immediate );
    
    void          display  ( Displayer &dp ) const;
    void          write    ( Ast &ast ) const;

    Scope         scope;
    Vec<PNode *>  nodes;
};

} // namespace Pst
END_TL_NAMESPACE

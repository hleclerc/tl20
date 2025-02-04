#pragma once

#include "Scope.h"
#include "Node.h"

BEG_TL_NAMESPACE
namespace PreAst {

/** 
*/
class Block {
public:
    void          display( Displayer &ds ) const;
    void          write  ( AstWriter &aw ) const;

    Scope         scope;
    Vec<Node *>   nodes;
};

} // namespace PreAst
END_TL_NAMESPACE

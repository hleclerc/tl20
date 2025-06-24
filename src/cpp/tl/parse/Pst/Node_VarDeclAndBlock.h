
#pragma once

#include "VarDecl.h"
#include "Block.h"

BEG_TL_NAMESPACE
namespace Pst {

/** 
    Shortcut to create data from a Variables declaration (as in `def f()`, `a := ...`, ...) and a Block

    Used notably to create the first argument for `operator def`, ...
*/
class Node_VarDeclAndBlock : public PNode {
public:
    using           PNode::PNode;
    
    virtual void    display     ( Displayer &ds ) const;
    virtual void    write       ( Ast &ast ) const;
   
    VarDecl*        var_decl;   ///<
    Block*          block;      ///<
};

} // namespace Pst
END_TL_NAMESPACE

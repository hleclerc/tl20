#pragma once

#include "Block.h"

BEG_TL_NAMESPACE
namespace Pst {

/** 
    node of a "pre ast" graph, used to make a final Ast for tokens
*/
class Node_Module : public PNode {
public:
    /**/          Node_Module( Tok::TNode *token );

    virtual void  display    ( Displayer &dp ) const;
    virtual void  write      ( Ast &ast ) const;

    Vec<GString>  global_variables;
    Block*        block;
};

} // namespace Pst
END_TL_NAMESPACE

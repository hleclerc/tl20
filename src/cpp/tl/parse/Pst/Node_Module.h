#pragma once

#include "Block.h"

BEG_TL_NAMESPACE
namespace Pst {

/** 
    node of a "pre ast" graph, used to make a final Ast for tokens
*/
class Node_Module : public Node {
public:
    /**/          Node_Module( Tok::Node *token );

    virtual void  display    ( Displayer &ds ) const;
    virtual void  write      ( Ast::Writer &aw ) const;

    Vec<Str>      global_variables;
    Block*        block;
};

} // namespace Pst
END_TL_NAMESPACE

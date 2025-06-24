#pragma once

#include "PNode.h"

BEG_TL_NAMESPACE
namespace Pst {

/** 
    node of a "pre ast" graph, used to make a final Ast for tokens
*/
class Arg {
public:
    /**/            Arg    ( Tok::TNode *token, Str name = {}, PNode *node = nullptr );

    void            display( Displayer &ds ) const;
    void            write  ( Ast &ast ) const;

    Tok::TNode*     token; 
    Str             name;
    PNode*          node;
};

} // namespace Pst
END_TL_NAMESPACE

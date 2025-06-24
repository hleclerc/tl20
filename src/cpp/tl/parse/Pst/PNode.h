#pragma once

#include "../Tok/TNode.h"
class Ast;

BEG_TL_NAMESPACE
namespace Pst {

/** 
    node of a "pre ast" graph, used to make a final Ast for tokens
*/
class PNode {
public:
    /**/            PNode     ( Tok::TNode *token );
    virtual        ~PNode     ();

    virtual void    display  ( Displayer &ds ) const = 0;
    virtual void    write    ( Ast &ast ) const = 0;

    Tok::TNode*     token; 
};

} // namespace Pst
END_TL_NAMESPACE

#pragma once

#include "PNode.h"

BEG_TL_NAMESPACE
namespace Pst {

/** 
    node of a "pre ast" graph, used to make a final Ast for tokens
*/
class Node_String : public PNode {
public:
    /**/            Node_String  ( Tok::TNode *token, const Str &content );

    virtual void    display      ( Displayer &dp ) const;
    virtual void    write        ( Ast &ast ) const;

    Str             content;        ///< 
};

} // namespace Pst
END_TL_NAMESPACE

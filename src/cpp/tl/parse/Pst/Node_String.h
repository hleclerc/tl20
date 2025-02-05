#pragma once

#include "Node.h"

BEG_TL_NAMESPACE
namespace Pst {

/** 
    node of a "pre ast" graph, used to make a final Ast for tokens
*/
class Node_String : public Node {
public:
    /**/            Node_String  ( Tok::Node *token, const Str &content );

    virtual void    display      ( Displayer &ds ) const;
    virtual void    write        ( Ast::Writer &aw ) const;

    Str             content;        ///< 
};

} // namespace Pst
END_TL_NAMESPACE

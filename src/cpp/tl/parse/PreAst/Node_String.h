#pragma once

#include "Node.h"

BEG_TL_NAMESPACE
namespace PreAst {

/** 
    node of a "pre ast" graph, used to make a final Ast for tokens
*/
class Node_String : public Node {
public:
    /**/            Node_String  ( TlToken *token, const Str &content );

    virtual void    display      ( Displayer &ds ) const;
    virtual void    write        ( AstWriter &aw ) const;

    Str             content;        ///< 
};

} // namespace PreAst
END_TL_NAMESPACE

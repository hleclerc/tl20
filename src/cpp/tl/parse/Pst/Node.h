#pragma once

#include "../../support/Displayer.h"
#include "../Ast/Writer.h"
#include "../Tok/Node.h"

BEG_TL_NAMESPACE
namespace Pst {

/** 
    node of a "pre ast" graph, used to make a final Ast for tokens
*/
class Node {
public:
    /**/            Node     ( Tok::Node *token );
    virtual        ~Node     ();

    virtual void    display  ( Displayer &ds ) const = 0;
    virtual void    write    ( Ast::Writer &aw ) const = 0;

    Tok::Node*      token; 
};

} // namespace Pst
END_TL_NAMESPACE

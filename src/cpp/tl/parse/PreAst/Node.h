#pragma once

#include "../../support/Displayer.h"
#include "../AstWriter.h"

BEG_TL_NAMESPACE
class TlToken;
namespace PreAst {

/** 
    node of a "pre ast" graph, used to make a final Ast for tokens
*/
class Node {
public:
    /**/            Node    ( TlToken *token );
    virtual        ~Node    ();

    virtual void    display ( Displayer &ds ) const = 0;
    virtual void    write   ( AstWriter &aw ) const = 0;

    TlToken*        token; 
};

} // namespace PreAst
END_TL_NAMESPACE

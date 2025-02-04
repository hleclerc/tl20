#pragma once

#include "Node.h"

BEG_TL_NAMESPACE
namespace PreAst {

/** 
    node of a "pre ast" graph, used to make a final Ast for tokens
*/
class Arg {
public:
    /**/            Arg    ( TlToken *token, Str name = {}, Node *node = nullptr );

    void            display( Displayer &ds ) const;
    void            write  ( AstWriter &aw ) const;

    TlToken*        token; 
    Str             name;
    Node*           node;
};

} // namespace PreAst
END_TL_NAMESPACE

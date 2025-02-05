#pragma once

#include "Node.h"

BEG_TL_NAMESPACE
namespace Pst {

/** 
    node of a "pre ast" graph, used to make a final Ast for tokens
*/
class Arg {
public:
    /**/            Arg    ( Tok::Node *token, Str name = {}, Node *node = nullptr );

    void            display( Displayer &ds ) const;
    void            write  ( Ast::Writer &aw ) const;

    Tok::Node*        token; 
    Str             name;
    Node*           node;
};

} // namespace Pst
END_TL_NAMESPACE

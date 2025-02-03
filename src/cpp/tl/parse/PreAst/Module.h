#pragma once

#include "Block.h"

BEG_TL_NAMESPACE
namespace PreAst {

/** 
    node of a "pre ast" graph, used to make a final Ast for tokens
*/
class Module : public Block {
public:
    /**/            Module   ( TlToken *token );

    virtual void    display  ( Displayer &ds ) const;
    virtual void    write    ( AstWriter &aw ) const;

    Vec<Str>        global_variables;
};

} // namespace PreAst
END_TL_NAMESPACE

#pragma once

#include "Block.h"

BEG_TL_NAMESPACE
namespace PreAst {

/** 
*/
class Module {
public:
    virtual void    display  ( Displayer &ds ) const;
    virtual void    write    ( AstWriter &aw ) const;

    Vec<Str>        global_variables;
    Block           block;
};

} // namespace PreAst
END_TL_NAMESPACE

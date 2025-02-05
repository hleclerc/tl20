#pragma once

#include "Block.h"

BEG_TL_NAMESPACE
namespace Pst {

/** 
*/
class Module {
public:
    virtual void    display  ( Displayer &ds ) const;
    virtual void    write    ( Ast::Writer &aw ) const;

    Vec<Str>        global_variables;
    Block           block;
};

} // namespace Pst
END_TL_NAMESPACE

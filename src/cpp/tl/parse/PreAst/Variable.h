#pragma once

#include "../../support/Displayer.h"

BEG_TL_NAMESPACE
namespace PreAst {

/** 
    Variable in a PreAstScope
    * orig >= 0 means that it comes from a parent scope
    * org == -1 means new variable
*/
class Variable {
public:
    void    display         ( Displayer &ds ) const { ds << name; }

    Str     name;
    int     orig;
};

} // namespace PreAst
END_TL_NAMESPACE

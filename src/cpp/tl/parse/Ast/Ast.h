#pragma once

#include "../../support/containers/Vec.h"
#include "AString.h"

BEG_TL_NAMESPACE

/**
    Abstract syntax tree.


 */
class Ast {
public:
    AString      new_AString( StrView );

private:
    friend class AString;

    Vec<char>    strings;
    Vec<PI8>     nodes;
};

END_TL_NAMESPACE

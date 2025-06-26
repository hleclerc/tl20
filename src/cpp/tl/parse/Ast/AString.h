#pragma once

#include "../../support/common_types.h"
#include "../../support/Displayer.h"

BEG_TL_NAMESPACE
class Ast;

/**
    String stored in an Ast
 */
class AString {
public:
    operator     StrView() const;

    void         display( Displayer &dp ) const;
    PI           size   () const { return _size; }

private:
    friend class Ast;

    PI           _offset;
    PI           _size;
    const Ast*   _ast;
};

END_TL_NAMESPACE

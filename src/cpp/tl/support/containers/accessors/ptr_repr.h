#pragma once

#include "../../common_types.h"
#include "../Opt.h"

BEG_TL_NAMESPACE

/// generic way to find items of an objects. Tries a.for_each_item(...), a.size(), ...
Opt<Str> ptr_repr( auto *f )  {
    if ( f )
        return std::to_string( PI( f ) );
    return {};
}

END_TL_NAMESPACE

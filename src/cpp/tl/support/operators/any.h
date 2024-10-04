#pragma once

#include "../tl_namespace.h"

BEG_TL_NAMESPACE

///
auto any( auto &&vec, auto &&func, const auto &...args ) {
    for( const auto &val : vec )
        if ( func( val, args... ) )
            return true;
    return false;
}

END_TL_NAMESPACE

#pragma once

#include "../tl_namespace.h"

BEG_TL_NAMESPACE

/// scalar product
auto sgn( auto &&a ) {
    return ( a > 0 ) - ( a < 0 );
}

END_TL_NAMESPACE

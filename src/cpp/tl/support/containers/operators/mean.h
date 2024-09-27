#pragma once

#include "sum.h"

BEG_TL_NAMESPACE

/// binary
constexpr auto mean( auto &&a ) {
    return sum( FORWARD( a ) ) / a.size();
}

END_TL_NAMESPACE

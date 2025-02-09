#pragma once

#include "default_operators.h"
#include <cmath>

BEG_TL_NAMESPACE

/// max(abs(.))
auto norm_inf( auto &&a ) {
    using std::abs;
    using std::max;
    auto res = abs( a[ 0 ] );
    for( std::size_t i = 1; i < a.size(); ++i )
        res = max( res, abs( a[ i ] ) );
    return res;
}

END_TL_NAMESPACE

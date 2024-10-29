#pragma once

#include "default_operators.h"
#include <cmath>

BEG_TL_NAMESPACE

/// sum(abs(.))
auto norm_1( auto &&a ) {
    using std::abs;
    auto res = abs( a[ 0 ] );
    for( std::size_t i = 1; i < a.size(); ++i )
        res += abs( a[ i ] );
    return res;
}

END_TL_NAMESPACE

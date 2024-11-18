#pragma once

#include "../common_types.h"
#include "default_operators.h"
#include <cmath>

BEG_TL_NAMESPACE

///
struct Functor_ceil {
    auto operator()( auto &&...a ) const;
};

/// unary
constexpr auto ceil( auto &&a ) {
    //DEFAULT_UNA_OPERATOR_CODE( ceil )

    if constexpr ( std::is_integral_v<DECAYED_TYPE_OF(a)> )
        return a;
    else
        return std::ceil( a );
}

/// binary
constexpr auto ceil( auto &&a, auto &&b ) {
    DEFAULT_BIN_OPERATOR_CODE( ceil )

    return ceil( ( a + b - 1 ) / b ) * b;
}

constexpr PI ceil( PI a, PI b ) {
    return ( a + b - 1 ) / b * b;
}

///
auto Functor_ceil::operator()( auto &&...a ) const { return ceil( FORWARD( a )... ); }

END_TL_NAMESPACE

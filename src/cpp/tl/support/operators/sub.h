#pragma once

#include "../containers/IsAlwaysZero.h"
#include "default_operators.h"

BEG_TL_NAMESPACE

struct Functor_sub {
    auto operator()( auto &&a, auto &&b ) const { return FORWARD( a ) - FORWARD( b ); }
};

constexpr auto sub( auto &&a, auto &&b ) {
    // 0 - b
    if constexpr( IsAlwaysZero<DECAYED_TYPE_OF( a )>::value ) {
        return - FORWARD( b );
    } else

    // a - 0
    if constexpr( IsAlwaysZero<DECAYED_TYPE_OF( b )>::value ) {
        return FORWARD( a );
    } else

    DEFAULT_BIN_OPERATOR_CODE_SIGN( sub, - )

    STATIC_ASSERT_IN_IF_CONSTEXPR( 0, "found no way to call sub" );
}

END_TL_NAMESPACE

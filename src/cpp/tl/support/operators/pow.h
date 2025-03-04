#pragma once

#include "default_operators.h"

BEG_TL_NAMESPACE

struct Functor_pow_ {
    auto operator()( auto &&a, auto &&b ) const { using namespace std; return pow( FORWARD( a ), FORWARD( b ) ); }
};

constexpr auto pow_( auto &&a, auto &&b ) {
    // // 0 * b
    // if constexpr( IsAlwaysZero<DECAYED_TYPE_OF( a )>::value ) {
    //     return FORWARD( a );
    // } else

    // // a * 0
    // if constexpr( IsAlwaysZero<DECAYED_TYPE_OF( b )>::value ) {
    //     return FORWARD( b );
    // } else

    DEFAULT_BIN_OPERATOR_CODE( pow_ )

    STATIC_ASSERT_IN_IF_CONSTEXPR( 0, "found no way to call pow" );
}

END_TL_NAMESPACE

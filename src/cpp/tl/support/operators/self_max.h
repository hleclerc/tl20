#pragma once

// #include "default_operators.h"
// #include "../IsAlwaysZero.h"
#include "../tl_namespace.h"
#include <algorithm>

BEG_TL_NAMESPACE

// struct Functor_self_mul {
//     static auto type_name() { return "TL_NAMESPACE::Functor_self_mul"; }
//     auto op( auto &&a, auto &&b ) const { return FORWARD( a ) * FORWARD( b ); }
//     auto operator()( auto &a, auto &&b ) const { return self_mul( a, FORWARD( b ) ); }
// };

constexpr auto self_max( auto &a, auto &&b ) {
    using namespace std;
    a = max( a, b );
}

END_TL_NAMESPACE

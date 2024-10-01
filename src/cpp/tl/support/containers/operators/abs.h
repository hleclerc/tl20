#pragma once

#include "default_operators.h"

BEG_TL_NAMESPACE

struct Functor_abs {
    auto operator()( auto &&a ) const { using namespace std; return abs( a ); }
};

auto abs( auto &&a ) { DEFAULT_UNA_OPERATOR_CODE( abs, abs ) STATIC_ASSERT_IN_IF_CONSTEXPR( a, "found not way to call abs" ); }

END_TL_NAMESPACE

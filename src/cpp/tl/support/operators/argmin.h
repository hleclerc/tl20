#pragma once

#include "../common_macros.h"
#include "../common_types.h"
// #include "../TensorOrder.h"

BEG_TL_NAMESPACE

/// seq argmin
std::size_t argmin( auto &&a )/* requires ( TensorOrder<DECAYED_TYPE_OF( a )>::value == 1 )*/ {
    auto res = 0;
    for( PI i = 1; i < a.size(); ++i )
        if ( a[ i ] < a[ res ] )
            res = i;
    return res;
}

END_TL_NAMESPACE

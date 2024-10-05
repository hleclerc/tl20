#pragma once

#include "../containers/TensorOrder.h"
#include "../common_macros.h"
#include "../common_types.h"

BEG_TL_NAMESPACE

/// seq argmin
std::size_t argmax( auto &&a ) requires ( TensorOrder<DECAYED_TYPE_OF( a )>::value == 1 ) {
    auto res = 0;
    for( PI i = 1; i < a.size(); ++i )
        if ( a[ res ] < a[ i ] )
            res = i;
    return res;
}

END_TL_NAMESPACE

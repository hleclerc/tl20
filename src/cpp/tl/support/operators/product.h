#pragma once

#include "../common_macros.h"
#include "../common_types.h"

BEG_TL_NAMESPACE

/// seq any
auto product( auto &&a ) -> DECAYED_TYPE_OF( a[ 0 ] ) {
    if ( ! a.size() )
        return 1;

    auto res = a[ 0 ];
    for( PI i = 1; i < a.size(); ++i )
        res *= a[ i ];
    return res;
}

END_TL_NAMESPACE

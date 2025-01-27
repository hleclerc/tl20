#pragma once

#include "containers/Vec.h"

BEG_TL_NAMESPACE

template<class V,class Func>
auto map_vec( const V &vec, Func &&func ) {
    using R = DECAYED_TYPE_OF( func( vec[ 0 ] ) );
    Vec<R> res( FromReservationSize(), vec.size() );
    for( const auto &value : vec )
        res << func( value );
    return res;
}

END_TL_NAMESPACE

#pragma once

#include "CtInt.h"

BEG_TL_NAMESPACE

///
template<int beg,int end>
struct CtRange {
    static void for_each_item( auto &&f ) {
        if constexpr ( beg < end ) {
            f( CtInt<beg>() );
            CtRange<beg + 1, end>::for_each_item( FORWARD( f ) );
        }
    }
};

END_TL_NAMESPACE

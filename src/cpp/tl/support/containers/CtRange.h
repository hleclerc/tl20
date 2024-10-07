#pragma once

#include "CtInt.h"

BEG_TL_NAMESPACE

///
template<int beg,int end>
struct CtRange {
    static void for_each_item( auto &&f ) {
        if constexpr ( end - beg == 3 ) {
            f( CtInt<beg+0>() );
            f( CtInt<beg+1>() );
            f( CtInt<beg+2>() );
        } else if constexpr ( beg < end ) {
            f( CtInt<beg>() );
            CtRange<beg + 1, end>::for_each_item( FORWARD( f ) );
        }
    }
};

END_TL_NAMESPACE

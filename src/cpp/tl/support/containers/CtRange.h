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

    static bool find_item( auto &&f ) {
        if constexpr ( end - beg == 3 ) {
            if ( f( CtInt<beg+0>() ) ) return true;
            if ( f( CtInt<beg+1>() ) ) return true;
            if ( f( CtInt<beg+2>() ) ) return true;
            return false;
        } else if constexpr ( beg < end ) {
            if ( f( CtInt<beg>() ) )
                return true;
            return CtRange<beg + 1, end>::find_item( FORWARD( f ) );
        }
        // hum
        return false;
    }
};

END_TL_NAMESPACE

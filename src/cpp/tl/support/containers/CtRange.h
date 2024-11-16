#pragma once

#include "CtInt.h"

BEG_TL_NAMESPACE

///
template<int beg,int end>
struct CtRange {
    static void for_each_item( auto &&f ) {
        f( CtInt<beg>() );
        CtRange<beg + 1, end>::for_each_item( FORWARD( f ) );
    }

    static bool find_item( auto &&f ) {
        if ( f( CtInt<beg>() ) )
            return true;
        return CtRange<beg + 1, end>::find_item( FORWARD( f ) );
    }
};

template<int end>
struct CtRange<end,end> {
    static void for_each_item( auto &&f ) {
    }

    static bool find_item( auto &&f ) {
        return false;
    }
};

END_TL_NAMESPACE

#pragma once

#include "../common_types.h"

BEG_TL_NAMESPACE

template<class T>
PI64 read_number( StrView &str, T base = 10 ) {
    T res = 0;
    while ( str.size() && str[ 0 ] >= '0' && str[ 0 ] <= '9' ) {
        res = base * res + ( str[ 0 ] - '0' );
        str.remove_prefix( 1 );
    }
    return res;
}

END_TL_NAMESPACE

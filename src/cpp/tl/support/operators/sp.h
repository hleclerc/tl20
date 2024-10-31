#pragma once

#include "default_operators.h"
#include "../type_info/zero.h"

BEG_TL_NAMESPACE

/// scalar product
auto sp( auto &&a, auto &&b ) {
    using TR = DECAYED_TYPE_OF( a[ 0 ] * b[ 0 ] );
    TR res = zero( CtType<TR>() );
    for( std::size_t i = 0; i < a.size(); ++i )
        res += a[ i ] * b[ i ];
    return res;
}

/// scalar product
auto sp( auto &&a, auto &&b, auto size ) {
    using TR = DECAYED_TYPE_OF( a[ 0 ] * b[ 0 ] );
    TR res = zero( CtType<TR>() );
    for( std::size_t i = 0; i < size; ++i )
        res += a[ i ] * b[ i ];
    return res;
}

END_TL_NAMESPACE

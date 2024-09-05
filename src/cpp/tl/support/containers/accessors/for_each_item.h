#pragma once

#include "../../STATIC_ASSERT_IN_IF_CONSTEXPR.h"
#include "../../common_macros.h"

BEG_TL_NAMESPACE

void for_each_item( const auto &a, const auto &f ) requires ( requires { a.for_each_item( f ); } || requires { a.size(); a[ 0 ]; } || requires { a.begin(); a.end(); } ) {
    if constexpr ( requires { a.for_each_item( f ); } ) {
        a.for_each_item( f );
        return;
    } else

    if constexpr ( requires { a.size(); a[ 0 ]; } ) {
        for( auto s = a.size(), i = 0; i < s; ++i )
            f( a[ i ] );
        return;
    } else

    if constexpr ( requires { a.begin(); a.end(); } ) {
        for( const auto &v : a )
            f( v );
        return;
    } else

    STATIC_ASSERT_IN_IF_CONSTEXPR( 0, "" );
}

END_TL_NAMESPACE

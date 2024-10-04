#pragma once

#include "../../STATIC_ASSERT_IN_IF_CONSTEXPR.h"
#include "../../common_macros.h"

BEG_TL_NAMESPACE

void for_each_attribute( const auto &a, const auto &f ) requires ( requires { a.for_each_attribute( f ); } ) {
    if constexpr ( requires { a.for_each_attribute( f ); } ) {
        a.for_each_attribute( f );
        return;
    } else

    STATIC_ASSERT_IN_IF_CONSTEXPR( 0, "" );
}

END_TL_NAMESPACE

#pragma once

#include "../../STATIC_ASSERT_IN_IF_CONSTEXPR.h"
#include "../../common_macros.h"

BEG_TL_NAMESPACE

/// generic way to find attributes of an objects. For now, it tries a.for_each_attribute(...)
void for_each_attribute( const auto &a, const auto &f ) requires ( requires { a.for_each_attribute( f ); } ) {
    if constexpr ( requires { a.for_each_attribute( f ); } ) {
        a.for_each_attribute( f );
        return;
    } else

    STATIC_ASSERT_IN_IF_CONSTEXPR( 0, "" );
}

// auto _for_each_attribute( auto &&func, std::string_view names, const auto &...values ) { ( func( read_arg_name( names ), values ), ... ); }
// auto _append_attributes( Displayer &ds, std::string_view names, const auto &...values ) { ( ds.append_attribute( read_arg_name( names ), values ), ... ); }

// #define STD_TL_TYPE_INFO( NAME, INCL, ... ) public: void for_each_attribute( auto &&func ) const { _for_each_attribute( func, #__VA_ARGS__, ##__VA_ARGS__ ); }

END_TL_NAMESPACE

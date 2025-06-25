#pragma once

#include "containers/accessors/for_each_attribute.h"
#include "containers/accessors/for_each_item.h"

#include "display/Displayer.h"

BEG_TL_NAMESPACE

// declarations
void display_object( Displayer &dp, auto &&item );
void display_array ( Displayer &dp, auto &&item );

/// display content of a generic item. Can be surdefined if the default definition is not good enough
void display( Displayer &dp, auto &&item ) {
    if constexpr ( requires { for_each_attribute( item, [&]( StrView, const auto & ) {} ); } )
        display_object( dp, item );
    else if constexpr ( requires { for_each_item( item, [&]( const auto & ) {} ); } )
        display_array( dp, item );
    else if constexpr ( requires { *dp.os << item; } )
        *dp.os << item;
    else
        STATIC_ASSERT_IN_IF_CONSTEXPR( 0, "found no way to display object" );
}

// definitions
void display_object( Displayer &dp, auto &&item ) {
    dp.incr();
    for_each_attribute( item, [&]( StrView name, const auto &value ) {
        dp.new_item();
        dp.write_attr_name( name );
        display( dp, value );
    } );
    dp.decr();
}

void display_array( Displayer &dp, auto &&item ) {
    dp.incr();
    for_each_item( item, [&]( const auto &value ) {
        dp.new_item();
        display( dp, value );
    } );
    dp.decr();
}



END_TL_NAMESPACE

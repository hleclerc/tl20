#pragma once

#include "containers/accessors/for_each_attribute.h"
#include "containers/accessors/for_each_item.h"

#include "display/DisplayParameters.h"
#include "display/DisplayContext.h"

BEG_TL_NAMESPACE

// declarations
void display_object( std::ostream &os, auto &&item, DisplayContext &dc );
void display_array ( std::ostream &os, auto &&item, DisplayContext &dc );

/// display content of a generic item. Can be surdefined if the default definition is not good enough
void display_item( std::ostream &os, auto &&item, DisplayContext &dc ) {
    if constexpr ( requires { for_each_attribute( item, [&]( StrView, const auto & ) {} ); } )
        display_object( os, item, dc );
    else if constexpr ( requires { for_each_item( item, [&]( const auto & ) {} ); } )
        display_array( os, item, dc );
    else if constexpr ( requires { os << item; } )
        os << item;
    else
        STATIC_ASSERT_IN_IF_CONSTEXPR( 0, "found no way to display object" );
}

///
void display( std::ostream &os, auto &&item, const DisplayParameters &dp = {} ) {
    DisplayContext dc;
    dc.dp = &dp;
    display_item( os, FORWARD( item ), dc );
}

// definitions
void display_object( std::ostream &os, auto &&item, DisplayContext &dc ) {
    dc.incr();
    for_each_attribute( item, [&]( StrView name, const auto &value ) {
        dc.new_item( os );
        os << name << ":";
        display_item( os, value, dc );
    } );
    dc.decr();
}

void display_array( std::ostream &os, auto &&item, DisplayContext &dc ) {
    dc.incr();
    for_each_item( item, [&]( const auto &value ) {
        dc.new_item( os );
        display_item( os, value, dc );
    } );
    dc.decr();
}



END_TL_NAMESPACE

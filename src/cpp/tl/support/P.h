#pragma once

#include "string/read_arg_name.h"
#include "Displayer.h"
#include "TODO.h"

#include <iostream>
#include <mutex>

BEG_TL_NAMESPACE
template<class Value> std::string type_name();

void __print_with_mutex( std::ostream &os, const DisplayParameters &prf, std::string_view arg_names, const auto &...arg_values ) {
    Displayer dp( &os, prf );
    auto get_item = [&]( const auto &arg_value ) {
        dp.new_item();
        dp.write_attr_name( read_arg_name( arg_names ) );
        display_item( dp, arg_value );
    };

    // call display
    static std::mutex m;
    m.lock();
    ( get_item( arg_values ), ... );
    dp.write_ptr_content();
    os << std::endl;
    m.unlock();
}

// template<class OS,class... ArgValues>
// void __print_types_with_mutex( OS &os, const DisplayParameters &dp, std::string_view arg_names, ArgValues &&...arg_values ) {
//     __print_with_mutex( os, dp, arg_names, type_name<decltype(arg_values)>()... );
// }

template<class... ArgValues>
void __show( std::string_view arg_names, ArgValues &&...arg_values ) {
    // create a root display item
    // Displayer ds;
    // ( ds.append_attribute( read_arg_name( arg_names ), arg_values ), ... );

    // ds.show();
    TODO;
}

#ifndef P
    // PRINT in cout
    #define P( ... ) \
        TL_NAMESPACE::__print_with_mutex( std::cout, DisplayParameters::for_debug_info(), #__VA_ARGS__, __VA_ARGS__ )

    // PRINT in cerr
    #define PE( ... ) \
        TL_NAMESPACE::__print_with_mutex( std::cerr, DisplayParameters::for_debug_info(), #__VA_ARGS__, __VA_ARGS__ )

    // PRINT in cout with options
    #define PO( VALUE, PARAMS ) \
        __print_with_mutex( std::cout, " -> ", ", ", PARAMS, #VALUE, VALUE )

    // PRINT in cout
    #define PT( ... ) \
        TL_NAMESPACE::__print_types_with_mutex( std::cout, DisplayParameters::for_debug_info(), #__VA_ARGS__, __VA_ARGS__ )

    // PRINT with .display in cout with options
    #define PD( VALUE, ... ) \
        ( VALUE ).display( __VA_ARGS__ ).display( std::cout  << #VALUE " -> \n" ) << std::endl

    // PRINT with file and line info
    #define PM( ... ) \
        __print_with_mutex( std::cout << __FILE__ << ':' << __LINE__, " -> ", ", ", {}, #__VA_ARGS__, __VA_ARGS__, WithSep{""},  )

    // Display graph
    #define SHOW( ... ) \
        TL_NAMESPACE::__show( #__VA_ARGS__, __VA_ARGS__ )

    // PRINT counter
    #define PC do { static int cpt = 0; PE( cpt++ ); } while ( false )
#endif

END_TL_NAMESPACE

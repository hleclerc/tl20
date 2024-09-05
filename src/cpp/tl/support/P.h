#pragma once

#include "string/read_arg_name.h"
#include "Displayer.h"
#include "TODO.h"

#include <iostream>
#include <mutex>

BEG_TL_NAMESPACE
template<class Value> std::string type_name();

void __print_with_mutex( std::ostream &os, const DisplayParameters &prf, std::string_view arg_names, const auto &...arg_values ) {
    // create a root display item
    Displayer ds;
    ( ds.append_attribute( read_arg_name( arg_names ), arg_values ), ... );

    // make a string
    Str out;
    ds.write_to( out, prf );

    // display it
    static std::mutex m;
    m.lock();    
    os << out << std::endl;
    m.unlock();
}

// template<class OS,class... ArgValues>
// void __print_types_with_mutex( OS &os, const DisplayParameters &dp, std::string_view arg_names, ArgValues &&...arg_values ) {
//     __print_with_mutex( os, dp, arg_names, type_name<decltype(arg_values)>()... );
// }

template<class... ArgValues>
void __show( const Str &arg_names, ArgValues &&...arg_values ) {
    // create a root display item
    // DisplayItemFactory ds;
    // DisplayItem *item = ds.new_object( {}, [&]( DisplayObjectFiller &dof ) {
    //     dof.add( arg_names, arg_values... );
    // } );

    // ds.show( item );
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

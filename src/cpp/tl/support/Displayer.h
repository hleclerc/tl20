#pragma once

#include "containers/accessors/for_each_attribute.h"
#include "containers/accessors/for_each_item.h"
#include "containers/accessors/ptr_repr.h"

#include "display/DisplayParameters.h"
#include "display/DisplayContext.h"

#include "string/read_arg_name.h"

#include "common_macros.h"
#include "ERROR.h"

#include <sstream>
#include <map>

BEG_TL_NAMESPACE
class Displayer;
void display_item( Displayer &dp, auto &&item );

/**

*/
class Displayer {
public:
    using      Parameters       = DisplayParameters;
    using      Context          = DisplayContext;
    using      PtrVec           = std::vector<Str>;
    using      PtrMap           = std::map<Str,PI>;
    using      Os               = std::ostream;
  
    /* */      Displayer        ( std::ostream *os, const Parameters &parameters = {} ) : parameters( parameters ) { context.os = os; context.beg_line = parameters.beg_line; }
  
    Displayer& operator<<       ( auto &&item ) { display_item( *this, FORWARD( item ) ); return *this; }

    void       write_ptr_content() { for( PI i = 0; i < pointer_vec.size(); ++i ) { new_item(); os() << pointer_vec[ i ]; } }
    Context    exchange_context ( Os *os ) { Context out = context; context.beg_line = parameters.beg_line; context.os = os; return out; }
    void       write_attr_name  ( StrView name ) { os() << name << ": "; }
    void       new_item         () { if ( ! std::exchange( context.first_item, false ) ) os() << "\n"; os() << context.beg_line; }
    void       incr             () { context.beg_line += "  "; }
    void       decr             () { context.beg_line.resize( context.beg_line.size() - 2 ); }
    Os&        os               () { return *context.os; }
 
 
    PtrMap     pointer_map;     ///< id -> num 
    PtrVec     pointer_vec;     ///< num -> pointed data repr
    Parameters parameters;      ///<
    Context    context;         ///<
};

// declarations
void display_object( Displayer &dp, auto &&item );
void display_array ( Displayer &dp, auto &&item );
void display_ptr   ( Displayer &dp, auto &&item );

/// display content of a generic item. Can be surdefined if the default definition is not good enough
void display_item( Displayer &dp, auto &&item ) {
    if constexpr ( requires { for_each_attribute( item, [&]( StrView, const auto & ) {} ); } )
        display_object( dp, item );
    else if constexpr ( requires { for_each_item( item, [&]( const auto & ) {} ); } )
        display_array( dp, item );
    else if constexpr ( requires { item.display( dp ); } )
        item.display( dp );
    else if constexpr ( requires { ptr_repr( item ); } )
        display_ptr( dp, item );
    else if constexpr ( requires { dp.os() << item; } )
        dp.os() << item;
    else
        ERROR( "found no way to display object" );
}

inline void display_item( Displayer &dp, const char *str ) {
    dp.os() << str;
}

inline void display_item( Displayer &dp, const Str &str ) {
    dp.os() << str;
}

inline void display_item( Displayer &dp, StrView str ) {
    dp.os() << str;
}

/// display 
void display( Displayer &dp, auto &&item ) {
    display_item( dp, FORWARD( item ) );
    dp.write_ptr_content();
}

// definitions
void display_attributes( Displayer &dp, StrView attr_names, auto &&...attr_values ) {
    auto get_item = [&]( const auto &attr_value ) {
        dp.new_item();
        dp.write_attr_name( read_arg_name( attr_names ) );
        display_item( dp, attr_value );
    };

    dp.incr();
    ( get_item( attr_values ), ... );
    dp.decr();
}

void display_object( Displayer &dp, auto &&item ) {
    dp.incr();
    for_each_attribute( item, [&]( StrView name, const auto &value ) {
        dp.new_item();
        dp.write_attr_name( name );
        display_item( dp, value );
    } );
    dp.decr();
}

void display_array( Displayer &dp, auto &&item ) {
    dp.incr();
    for_each_item( item, [&]( const auto &value ) {
        dp.new_item();
        display_item( dp, value );
    } );
    dp.decr();
}

void display_ptr( Displayer &dp, auto &&item ) {
    if ( Opt<Str> pr = ptr_repr( item ) ) {
        auto iter = dp.pointer_map.find( *pr );
        if ( iter == dp.pointer_map.end() ) {
            // pointed data
            std::ostringstream ss;
            DisplayContext oc = dp.exchange_context( &ss );
            display_item( dp, *item );
            dp.context = oc;
            
            // $id
            PI ind = dp.pointer_vec.size();
            iter = dp.pointer_map.insert( iter, { *pr, ind } );
            dp.pointer_vec.push_back( "$" + std::to_string( ind ) + ": " + ss.str() );
        }

        dp.os() << "$" << iter->second;
    } else
        dp << "null";
}

#define DS_OBJECT( NAME, ... ) \
    display_attributes( dp, #__VA_ARGS__, __VA_ARGS__ )

END_TL_NAMESPACE

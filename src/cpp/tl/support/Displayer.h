#pragma once

#include "containers/accessors/for_each_attribute.h"
#include "containers/accessors/for_each_item.h"
#include "display/DisplayParameters.h"
#include "memory/BumpPointerPool.h"
#include "string/read_arg_name.h"
#include "TODO.h"

#include <functional>
#include <sstream>
#include <map>

BEG_TL_NAMESPACE
class DisplayItem;

/**
 * @brief 
 * 
 */
class Displayer {
public:
    using           Pointers        = std::map<Str,DisplayItem *>;
    struct          Number          { Str numerator, denominator, shift, base_shift; }; ///< values are represented in base 10.
   
    /**/            Displayer       ();
    /**/           ~Displayer       ();

    void            set_next_name   ( StrView name ); ///< set the name of the next item to be appended
    void            set_next_type   ( StrView type ); ///< set the type of the next item to be appended
    T_T Displayer&  operator<<      ( const T &value ) { display( *this, value ); return *this; }
    virtual void    write_to        ( Str &out, const DisplayParameters &dp ) const;

    // helpers
    void            append_attribute( StrView name, const auto &value ) { set_next_name( name ); operator<<( value ); }
    void            append_pointer  ( bool valid, const Str &id, const std::function<void()> &cb );
    void            append_number   ( const Number &number );
    void            append_string   ( StrView str );
    void            append_array    ( const std::function<void()> &cb );

    void            start_array     ();
    void            end_array       ();

    void            start_object    ();
    void            end_object      ();

    DisplayItem*    last_container;
    Str             next_name;
    Str             next_type;
    Pointers        pointers;
    BumpPointerPool pool;
};

T_T Str pointer_repr( const T *ptr ) { return std::to_string( PI( ptr ) ); }

// STD_TL_TYPE_INFO
auto _for_each_attribute( auto &&func, std::string_view names, const auto &...values ) { ( func( read_arg_name( names ), values ), ... ); }
auto _append_attributes( Displayer &ds, std::string_view names, const auto &...values ) { ( ds.append_attribute( read_arg_name( names ), values ), ... ); }

#define STD_TL_TYPE_INFO( NAME, INCL, ... ) public: void for_each_attribute( auto &&func ) const { _for_each_attribute( func, #__VA_ARGS__, ##__VA_ARGS__ ); }
#define DS_OJBECT( ... ) { ds.start_object(); _append_attributes( ds, #__VA_ARGS__, ##__VA_ARGS__ ); ds.end_object(); }

// =======================================================================================================================================
void display( Displayer &ds, const Str&  str );
void display( Displayer &ds, StrView     str );
void display( Displayer &ds, const char* str );
void display( Displayer &ds, char        str );

void display( Displayer &ds, PI64        val );
void display( Displayer &ds, PI32        val );
void display( Displayer &ds, PI16        val );
void display( Displayer &ds, PI8         val );

void display( Displayer &ds, SI64        val );
void display( Displayer &ds, SI32        val );
void display( Displayer &ds, SI16        val );
void display( Displayer &ds, SI8         val );

void display( Displayer &ds, bool        val );

void display( Displayer &ds, FP80        val );
void display( Displayer &ds, FP64        val );
void display( Displayer &ds, FP32        val );

void display( Displayer &ds, const void* val );
void display( Displayer &ds, void*       val );

// // std::variant
// template<class... A>
// void display( Displayer &ds, const std::variant<A...> &value ) {
//     std::visit( [&]( const auto &v ) {
//         return ds.new_display_item( v );
//     }, value );
// }

// generic
template<class T>
void display( Displayer &ds, const T &value ) {
    // value.display
    if constexpr( requires { value.display( ds ); } ) {
        value.display( ds );
        return;
    }

    // for_each_attribute (for objects)
    else if constexpr( requires { value.for_each_attribute( []( const auto &, const auto & ) {} ); } ) {
        ds.start_object();
        value.for_each_attribute( [&]( const auto &name, const auto &attr ) {
            ds.set_next_name( name );
            ds << attr;
        } );
        ds.end_object();
        return;
    }

    // for_each_item (for arrays)
    else if constexpr( requires { for_each_item( value, []( const auto & ) {} ); } ) {
        ds.start_array();
        for_each_item( value, [&]( const auto &value ) {
            ds << value;
        } );
        ds.end_array();
        return;
    }

    // *value
    else if constexpr( requires { bool( value ), *value; } ) {
        ds.append_pointer( bool( value ), pointer_repr( value ), [&]() { ds << *value; } );
        return;
    }

    // value.to_string
    else if constexpr( requires { value.to_string(); } ) {
        ds << value.to_string();
        return;
    }

    // os << ...
    else if constexpr( requires ( std::ostream &os ) { os << value; } ) {
        std::ostringstream ss;
        ss << value;
        ds << ss.str();
        return;
    }

    // apply( ... );
    else if constexpr( requires { std::apply( []( const auto &...items ) {}, value ); } ) {
        ds.start_array();
        std::apply( [&]( const auto &...items ) {
            ( ds << ... << items );
        }, value );
        ds.end_array();
        return;
    }

    // T::template_type_name() (for empty structures)
    else if constexpr( requires { T::template_type_name(); } ) {
        // return ds.display( T::template_type_name() );
        TODO;
    }

    // T::type_name() (for empty structures)
    else if constexpr( requires { T::type_name(); } ) {
        // return ds.display( T::type_name() );
        TODO;
    }

    // value.display again (to get an error message)
    else
        return value.display( ds );
}


END_TL_NAMESPACE

#include "display/DisplayItem_Pointer.h"
#include "display/DisplayItem_String.h"
#include "display/DisplayItem_Number.h"
#include "display/DisplayItem_List.h"
#include "display/DisplayContext.h"
#include "Displayer.h"
#include <format>
// #include "TODO.h"

BEG_TL_NAMESPACE

Displayer::Displayer() {
    last_container = pool.create<DisplayItem_List>();
}

Displayer::~Displayer() {
}

void Displayer::set_next_name( StrView name ) {
    if ( next_name.empty() )
        next_name = name;
}

void Displayer::set_next_type( StrView type ) {
    if ( next_type.empty() )
        next_type = type;
}

void Displayer::append_number( const Number &number ) {
    auto *res = pool.create<DisplayItem_Number>();
    res->name = std::exchange( next_name, {} );
    res->type = std::exchange( next_type, {} );
    last_container->append( res );
    
    res->denominator = number.denominator;
    res->base_shift  = number.base_shift;
    res->numerator   = number.numerator;
    res->shift       = number.shift;
}

void Displayer::append_string( StrView str ) {
    auto *res = pool.create<DisplayItem_String>();
    res->name = std::exchange( next_name, {} );
    res->type = std::exchange( next_type, {} );
    last_container->append( res );

    res->str = str;
}

void Displayer::append_array( const std::function<void()> &cb ) {
    start_array();
    cb();
    end_array();
}

void Displayer::write_to( Str &out, const DisplayParameters &prf ) const {
    DisplayContext ctx;
    last_container->write_to( out, ctx, prf );

    // ensure endline if necessary
    if ( prf.ensure_endline && ! out.ends_with( "\n" ) )
        out += '\n';
}

void Displayer::append_pointer( bool valid, const Str &id, const std::function<void()> &cb ) {
    auto iter = pointers.find( id );
    if ( iter == pointers.end() ) {
        auto *res = pool.create<DisplayItem_Pointer>();
        res->name = std::exchange( next_name, {} );
        res->type = std::exchange( next_type, {} );

        res->parent = last_container;
        last_container = res;

        iter = pointers.insert( iter, { id, res } );

        if ( valid )
            cb();

        last_container = last_container->parent;
    }

    last_container->append( iter->second );
}

void Displayer::start_object() {
    auto *res = pool.create<DisplayItem_List>();
    res->name = std::exchange( next_name, {} );
    res->type = std::exchange( next_type, {} );
    last_container->append( res );

    res->is_an_object = true;

    res->parent = last_container;
    last_container = res;
}

void Displayer::end_object() {
    last_container = last_container->parent;
}

void Displayer::start_array() {
    auto *res = pool.create<DisplayItem_List>();
    res->name = std::exchange( next_name, {} );
    res->type = std::exchange( next_type, {} );
    last_container->append( res );

    res->parent = last_container;
    last_container = res;
}

void Displayer::end_array() {
    last_container = last_container->parent;
}

// ------------------------------------------------------------------------------------------------
void display( Displayer &ds, const Str&  str ) { ds.append_string( str ); }
void display( Displayer &ds, StrView     str ) { ds.append_string( str ); }
void display( Displayer &ds, const char* str ) { ds.append_string( str ); }
void display( Displayer &ds, char        str ) { ds.append_string( { &str, 1 } ); }

void display( Displayer &ds, PI64        val ) { ds.append_number( { .numerator = std::format( "{}", val ), .denominator = "1", .shift = "0", .base_shift = "2" } ); }
void display( Displayer &ds, PI32        val ) { ds.append_number( { .numerator = std::format( "{}", val ), .denominator = "1", .shift = "0", .base_shift = "2" } ); }
void display( Displayer &ds, PI16        val ) { ds.append_number( { .numerator = std::format( "{}", val ), .denominator = "1", .shift = "0", .base_shift = "2" } ); }
void display( Displayer &ds, PI8         val ) { ds.append_number( { .numerator = std::format( "{}", val ), .denominator = "1", .shift = "0", .base_shift = "2" } ); }

void display( Displayer &ds, SI64        val ) { ds.append_number( { .numerator = std::format( "{}", val ), .denominator = "1", .shift = "0", .base_shift = "2" } ); }
void display( Displayer &ds, SI32        val ) { ds.append_number( { .numerator = std::format( "{}", val ), .denominator = "1", .shift = "0", .base_shift = "2" } ); }
void display( Displayer &ds, SI16        val ) { ds.append_number( { .numerator = std::format( "{}", val ), .denominator = "1", .shift = "0", .base_shift = "2" } ); }
void display( Displayer &ds, SI8         val ) { ds.append_number( { .numerator = std::format( "{}", val ), .denominator = "1", .shift = "0", .base_shift = "2" } ); }

void display( Displayer &ds, bool        val ) { ds.append_number( { .numerator = std::format( "{}", val ), .denominator = "1", .shift = "0", .base_shift = "2" } ); }

void display( Displayer &ds, FP80        val ) { ds.append_number( { .numerator = std::format( "{}", val ), .denominator = "1", .shift = "0", .base_shift = "2" } ); }
void display( Displayer &ds, FP64        val ) { ds.append_number( { .numerator = std::format( "{}", val ), .denominator = "1", .shift = "0", .base_shift = "2" } ); }
void display( Displayer &ds, FP32        val ) { ds.append_number( { .numerator = std::format( "{}", val ), .denominator = "1", .shift = "0", .base_shift = "2" } ); }

void display( Displayer &ds, const void* val ) { ds << std::to_string( PI( val ) ); }
void display( Displayer &ds, void*       val ) { ds << std::to_string( PI( val ) ); }


END_TL_NAMESPACE

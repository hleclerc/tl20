#include "display/DisplayItem_Number.h"
#include "display/DisplayItem_List.h"
#include "display/DisplayContext.h"
#include "Displayer.h"
// #include "TODO.h"

BEG_TL_NAMESPACE

Displayer::Displayer() {
    last_container = pool.create<DisplayItem_List>();
}

Displayer::~Displayer() {
}

void Displayer::set_next_name( const Str &name ) {
    next_name = name;
}

void Displayer::set_next_type( const Str &name ) {
    next_name = name;
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

void Displayer::append_list( const std::function<void()> &cb ) {
    start_list();
    cb();
    end_list();
}

void Displayer::write_to( Str &out, const DisplayParameters &prf ) const {
    DisplayContext ctx;
    last_container->write_to( out, ctx, prf );

    // ensure endline if necessary
    if ( prf.ensure_endline && ! out.ends_with( "\n" ) )
        out += '\n';
}

void Displayer::start_list() {
    auto *res = pool.create<DisplayItem_List>();
    res->name = std::exchange( next_name, {} );
    res->type = std::exchange( next_type, {} );
    last_container->append( res );

    res->parent = last_container;
    last_container = res;
}

void Displayer::end_list() {
    last_container = last_container->parent;
}

void display( Displayer &ds, SI32 value ) { ds.append_number( { .numerator = std::to_string( value ), .denominator = "1", .shift = "0", .base_shift = "2" } ); }


END_TL_NAMESPACE

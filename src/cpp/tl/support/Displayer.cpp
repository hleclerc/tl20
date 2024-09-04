#include "display/DisplayItem_Number.h"
#include "display/DisplayItem_List.h"
#include "display/DisplayContext.h"
#include "Displayer.h"

BEG_TL_NAMESPACE

Displayer::Displayer() {
    last = nullptr;
}

Displayer::~Displayer() {
}

void Displayer::set_next_name( const Str &name ) {
    next_name = name;
}

void Displayer::append_number( const Number &number ) {
    auto *res = pool.create<DisplayItem_Number>();
    res->name = std::exchange( next_name, {} );
    res->prev_sibling = res;
    last = res;
    
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
    if ( ! last )
        return;

    DisplayContext ctx;
    last->write_to( out, ctx, prf );

    if ( prf.ensure_endline && ! out.ends_with( "\n" ) )
        out += '\n';
}

void Displayer::start_list() {
    auto *res = pool.create<DisplayItem_List>();
    res->name = std::exchange( next_name, {} );
    res->prev_parent = res;
    last = res;
}

void Displayer::end_list() {
    last = last->prev_parent;
}

void display( Displayer &ds, SI32 value ) { ds.append_number( { .numerator = std::to_string( value ), .denominator = "1", .shift = "0", .base_shift = "2" } ); }


END_TL_NAMESPACE

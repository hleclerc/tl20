#include "DisplayItem_List.h"
// #include "../TODO.h"

BEG_TL_NAMESPACE

void DisplayItem_List::write_to( Str &out, DisplayContext &ctx, const DisplayParameters &prf ) const {
    // if no child
    if ( ! has_children() ) {
        out += "[]";
        return;
    }

    // opening delimiter
    bool may_need_a_space = false;
    if ( ! is_the_root_item() ) {
        may_need_a_space = true;
        out += '[';
    }

    // children
    for_each_child( [&]( DisplayItem *child ) {
        if ( ! child->is_the_first_child() )
            out += ',';

        if ( prf.add_spaces_for_reading && may_need_a_space )
            out += ' ';

        child->write_to( out, ctx, prf );

        may_need_a_space = true;
    } );

    // closing delimiter
    if ( ! is_the_root_item() ) {
        if ( prf.add_spaces_for_reading && may_need_a_space )
            out += ' ';
        out += ']';
    }
}

END_TL_NAMESPACE

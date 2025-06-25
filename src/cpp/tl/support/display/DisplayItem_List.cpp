#include "DisplayItem_List.h"

BEG_TL_NAMESPACE

bool DisplayItem_List::has_default_value() const {
    for( DisplayItem *item = last_child; item; item = item->prev_sibling )
        if ( ! item->has_default_value() )
            return false;
    return true;
}

bool DisplayItem_List::has_names() const {
    for( DisplayItem *item = last_child; item; item = item->prev_sibling )
        if ( ! item->name.empty() )
            return true;
    return false;
}

void DisplayItem_List::write_content_to( Str &out, DisplayContext &ctx, const DisplayParameters &prf ) const {
    // if no child
    if ( ! has_children() ) {
        out += is_an_object ? "{}" : "[]";
        return;
    }

    bool use_new_lines = false;
    if ( prf.use_new_lines && max_tensor_order() >= 1 )
        use_new_lines = true;

    // opening delimiter
    bool may_need_a_space = false;
    if ( ! is_the_root_item() ) {
        may_need_a_space = true;
        if ( use_new_lines == false )
            out += is_an_object ? '{' : '[';
    }

    Str beg_line;
    if ( use_new_lines && ! is_the_root_item() )
        ctx.incr();

    // children
    PI old_out_size = out.size();
    for_each_child( [&]( DisplayItem *child ) {
        if ( prf.skip_if_default_value && child->has_default_value() )
            return;
        
        if ( use_new_lines ) {
            if ( may_need_a_space && prf.always_display_delimiters )
                ctx.write_beg_line( out );
        } else {
            if ( old_out_size != out.size() ) {
                out += ',';
                old_out_size = out.size();
            }

            if ( prf.add_spaces_for_reading && may_need_a_space )
                out += ' ';
        }

        child->write_to( out, ctx, prf );

        may_need_a_space = true;
    } );

    if ( use_new_lines && ! is_the_root_item() )
        ctx.decr();

    // closing delimiter
    if ( ( use_new_lines == false || prf.always_display_delimiters ) && ! is_the_root_item() ) {
        if ( use_new_lines )
            ctx.write_beg_line( out );
        else if ( prf.add_spaces_for_reading && may_need_a_space )
            out += ' ';
        out += is_an_object ? '}' : ']';
    }
}

int DisplayItem_List::max_tensor_order() const {
    int res = 0;
    for_each_child( [&]( DisplayItem *child ) {
        res = std::max( res, child->max_tensor_order() );
    } );
    return res + 1; // + ( ! is_an_object );
}

END_TL_NAMESPACE

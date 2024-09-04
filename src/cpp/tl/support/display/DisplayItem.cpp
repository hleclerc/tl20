#include "DisplayItem.h"

BEG_TL_NAMESPACE

DisplayItem::~DisplayItem() {
}

bool DisplayItem::is_the_first_child() const{
    return ! prev_sibling;
}

bool DisplayItem::is_the_root_item() const {
    return ! parent;
}

void DisplayItem::for_each_child( const std::function<void( DisplayItem *child )> &f ) const {
    std::function<void( DisplayItem *child )> fecr = [&]( DisplayItem *child ) {
        if ( child ) {
            fecr( child->prev_sibling );
            f( child );
        }
    };
    fecr( last_child );
}

bool DisplayItem::has_children() const {
    return last_child;
}

void DisplayItem::append( DisplayItem *res ) {
    res->prev_sibling = last_child;
    last_child = res;
}

END_TL_NAMESPACE

#include "DisplayItem_Pointer.h"
// #include "../TODO.h"

BEG_TL_NAMESPACE

void DisplayItem_Pointer::write_content_to( Str &out, DisplayContext &ctx, const DisplayParameters &prf ) const {
    if ( last_child ) {
        last_child->write_content_to( out, ctx, prf );
    } else {
        out += "NULL";
    }
}

END_TL_NAMESPACE

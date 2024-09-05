#include "DisplayItem_String.h"

BEG_TL_NAMESPACE

void DisplayItem_String::write_content_to( Str &out, DisplayContext &ctx, const DisplayParameters &prf ) const {
    out += str;
}

END_TL_NAMESPACE

#include "DisplayItem_Number.h"

BEG_TL_NAMESPACE

void DisplayItem_Number::write_content_to( Str &out, DisplayContext &ctx, const DisplayParameters &prf ) const {
    out += numerator;
}

END_TL_NAMESPACE

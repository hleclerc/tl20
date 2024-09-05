#pragma once

#include "DisplayItem.h"

BEG_TL_NAMESPACE

/**
    values are internally represented in base 10.
*/
class DisplayItem_String : public DisplayItem {
public:
    virtual void write_content_to( Str &out, DisplayContext &ctx, const DisplayParameters &prf ) const override;

    Str          str;
};

END_TL_NAMESPACE

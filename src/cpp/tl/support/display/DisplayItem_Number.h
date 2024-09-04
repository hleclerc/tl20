#pragma once

#include "DisplayItem.h"

BEG_TL_NAMESPACE
class DisplayItem;

/**
    values are internally represented in base 10.
*/
class DisplayItem_Number : public DisplayItem {
public:
    virtual void write_to( Str &out, DisplayContext &ctx, const DisplayParameters &prf ) const override;

    Str          denominator;
    Str          base_shift;
    Str          numerator;
    Str          shift;
};

END_TL_NAMESPACE

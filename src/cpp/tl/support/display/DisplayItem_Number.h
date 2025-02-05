#pragma once

#include "DisplayItem.h"

BEG_TL_NAMESPACE

/**
    values are internally represented in base 10.
*/
class DisplayItem_Number : public DisplayItem {
public:
    virtual bool has_default_value() const override { return numerator.empty(); }
    virtual void write_content_to ( Str &out, DisplayContext &ctx, const DisplayParameters &prf ) const override;

    Str          denominator;
    Str          base_shift;
    Str          numerator;
    Str          shift;
};

END_TL_NAMESPACE

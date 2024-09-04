#pragma once

#include "DisplayItem.h"

BEG_TL_NAMESPACE
class DisplayItem;

/**
*/
class DisplayItem_List : public DisplayItem {
public:
    virtual void write_to( Str &out, DisplayContext &ctx, const DisplayParameters &prf ) const override;
};

END_TL_NAMESPACE

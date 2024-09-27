#pragma once

#include "DisplayItem.h"

BEG_TL_NAMESPACE

/**
*/
class DisplayItem_List : public DisplayItem {
public:
    virtual void write_content_to( Str &out, DisplayContext &ctx, const DisplayParameters &prf ) const override;
    virtual int  max_tensor_order() const override;

    bool         is_an_object    = false;
};

END_TL_NAMESPACE

#pragma once

#include "DisplayItem.h"

BEG_TL_NAMESPACE

/**
*/
class DisplayItem_Pointer : public DisplayItem {
public:
    virtual void write_content_to( Str &out, DisplayContext &ctx, const DisplayParameters &prf ) const override;
    virtual bool has_default_value () const override { return last_child == nullptr; }
};

END_TL_NAMESPACE

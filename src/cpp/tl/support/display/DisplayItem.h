#pragma once

#include "DisplayParameters.h"
#include "DisplayContext.h"

BEG_TL_NAMESPACE

/** 
*/
class DisplayItem {
public:
    virtual      ~DisplayItem  ();
    virtual void  write_to     ( Str &out, DisplayContext &ctx, const DisplayParameters &prf ) const = 0;

    DisplayItem*  prev_sibling = nullptr;
    DisplayItem*  prev_parent  = nullptr;
    Str           name;
};

END_TL_NAMESPACE

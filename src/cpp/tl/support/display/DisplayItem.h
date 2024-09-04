#pragma once

#include "DisplayParameters.h"
#include "DisplayContext.h"
#include <functional>

BEG_TL_NAMESPACE

/** 
*/
class DisplayItem {
public:
    virtual      ~DisplayItem       ();

    bool          is_the_first_child() const;
    bool          is_the_root_item  () const;
    void          for_each_child    ( const std::function<void( DisplayItem *child )> &f ) const;
    bool          has_children      () const;
    virtual void  write_to          ( Str &out, DisplayContext &ctx, const DisplayParameters &prf ) const = 0;
    void          append            ( DisplayItem *res );

    DisplayItem*  prev_sibling      = nullptr;
    DisplayItem*  last_child        = nullptr;
    DisplayItem*  parent            = nullptr;
    Str           name;
    Str           type;
};

END_TL_NAMESPACE

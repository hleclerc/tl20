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

    // modifications
    void          append            ( DisplayItem *res );

    // output
    virtual void  write_content_to  ( Str &out, DisplayContext &ctx, const DisplayParameters &prf ) const = 0;
    void          write_to          ( Str &out, DisplayContext &ctx, const DisplayParameters &prf ) const;

    // traversal
    void          for_each_child    ( const std::function<void( DisplayItem *child )> &f ) const;
    
    // properties
    bool          is_the_first_child() const;
    virtual bool  has_default_value () const = 0;
    bool          is_the_root_item  () const;
    virtual int   max_tensor_order  () const;
    bool          has_children      () const;

    // graph
    DisplayItem*  prev_sibling      = nullptr;
    DisplayItem*  last_child        = nullptr;
    DisplayItem*  parent            = nullptr;

    //
    bool          virtual_type;     ///< true if type
    Str           name;             ///< attribute name
    Str           type;             ///<
};

END_TL_NAMESPACE

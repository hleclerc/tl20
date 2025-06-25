#pragma once

#include "DisplayParameters.h"
#include "../common_types.h"
#include <ostream>

BEG_TL_NAMESPACE

/**

*/
class DisplayContext {
public:
    using DpPtr         = const DisplayParameters *;

    void  new_item      ( std::ostream &os );

    void  incr          ();
    void  decr          ();

    bool  first_item    = true;
    Str   beg_line      = {};
    DpPtr dp            = nullptr;
};

END_TL_NAMESPACE

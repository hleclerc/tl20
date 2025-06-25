#pragma once

#include "DisplayParameters.h"
#include "../common_types.h"
#include <ostream>

BEG_TL_NAMESPACE

/**

*/
class Displayer {
public:
    using Dp             = DisplayParameters;
    using Os             = std::ostream;

    void  write_attr_name( StrView name ) { *os << name << ": "; }
    void  new_item       () { if ( ! std::exchange( first_item, false ) ) *os << "\n"; *os << beg_line; }
    void  incr           () { beg_line += "  "; }
    void  decr           () { beg_line.resize( beg_line.size() - 2 ); }
   
    //   
    Dp    parameters;   
    Os*   os;   
   
    // context   
    bool  first_item     = true;
    Str   beg_line       = {};
};

END_TL_NAMESPACE

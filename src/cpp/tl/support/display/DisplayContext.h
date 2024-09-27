#pragma once

#include "../common_types.h"

BEG_TL_NAMESPACE

/**

*/
class DisplayContext {
public:
    void write_beg_line( Str &out ) { out += '\n'; out += beg_line; }

    void incr          () { beg_line += "  "; }
    void decr          () { beg_line.resize( beg_line.size() - 2 ); }

    Str  beg_line;
};

END_TL_NAMESPACE

#pragma once

#include "../common_types.h"

BEG_TL_NAMESPACE

/**

*/
class DisplayContext {
public:
    using      Os             = std::ostream;
    
    bool       first_item     = true;
    Str        beg_line       = {};
    Os*        os;
};

END_TL_NAMESPACE

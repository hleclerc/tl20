#pragma once

#include <tl/support/tl_namespace.h>

BEG_TL_NAMESPACE

/**

*/
class DisplayParameters {
public:
    static DisplayParameters for_debug_info() { return { .use_new_lines = true  }; }

    bool always_display_delimiters = false;
    bool add_spaces_for_reading = true;
    bool ensure_endline = false;
    bool use_new_lines = false;
};

END_TL_NAMESPACE

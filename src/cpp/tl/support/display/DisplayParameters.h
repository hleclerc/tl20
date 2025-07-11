#pragma once

#include "../common_types.h"

BEG_TL_NAMESPACE

/**

*/
class DisplayParameters {
public:
    static DisplayParameters for_debug_info() { return { .use_new_lines = true  }; }
    static DisplayParameters compact       () { return { .add_spaces_for_reading = false, .skip_if_default_value = true }; }

    bool   always_display_delimiters = false;
    bool   add_spaces_for_reading    = true;
    bool   skip_if_default_value     = false;
    bool   ensure_endline            = false;
    bool   use_new_lines             = false;
    Str    beg_line                  = {};
};

END_TL_NAMESPACE

#pragma once

#include "TlToken.h"

BEG_TL_NAMESPACE

/**

 */
struct TlParserStackItem {
    TlToken* token;

    int      closing_char;

    bool     on_a_new_line;
    int      newline_size;

    int      max_nb_children = 0;
    int      min_nb_children = 0;
    int      prio            = 0;
};

END_TL_NAMESPACE

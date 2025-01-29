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

    int      max_nb_children;
    int      prio;
};

END_TL_NAMESPACE

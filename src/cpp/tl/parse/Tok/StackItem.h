#pragma once

#include "Node.h"

BEG_TL_NAMESPACE
namespace Tok {

/**

 */
struct StackItem {
    Node*    token;

    int      closing_char;

    bool     on_a_new_line;
    int      newline_size;

    int      max_nb_children = 0;
    int      min_nb_children = 0;
    int      prio            = 0;
};

} // namespace Tok
END_TL_NAMESPACE

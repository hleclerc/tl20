#pragma once

#include "../common_types.h"

struct StyleOfTextLog {
    Str    indented_info_prefix  = "  ";
    bool   escape_sequences      = true;
    PI     max_line_size         = 80;

    bool   underline_extracts    = true;
    char   underline_middle_char = '^';
    char   underline_right_char  = '-';
    char   underline_left_char   = '-';
    Str    underline_ellipsis    = "...";
    int    offset_line           = 1;
    int    offset_col            = 1;
};

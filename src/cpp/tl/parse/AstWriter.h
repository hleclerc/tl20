#pragma once

#include "AstWriterStr.h"

BEG_TL_NAMESPACE

/** 
    Class to write an Ast 

*/
class AstWriter {
public:
    using        StrMap   = AstWriterStr::Map;

    /**/         AstWriter();

    AstWriterStr str      ( const Str &val );

    StrMap       str_map;
    PI           str_len;
};

END_TL_NAMESPACE

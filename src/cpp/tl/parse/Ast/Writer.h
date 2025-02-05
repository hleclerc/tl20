#pragma once

#include "WriterString.h"

BEG_TL_NAMESPACE
namespace Ast {

/** 
    Class to write an Ast 

*/
class Writer {
public:
    using        StrMap   = WriterString::Map;

    /**/         Writer   ();

    WriterString string      ( const Str &val );

private:
    StrMap       str_map;
    PI           str_len;
};

} // namespace Ast
END_TL_NAMESPACE

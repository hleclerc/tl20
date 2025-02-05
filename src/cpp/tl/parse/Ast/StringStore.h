#pragma once

#include "StringRef.h"

BEG_TL_NAMESPACE
namespace Ast {

/** 
    Class to write an Ast 

*/
class StringStore {
public:
    using     StrMap     = StringRef::Map;

    /**/      StringStore();

    StringRef string     ( const Str &val );

private:
    StrMap    str_map;
    PI        str_len;
};

} // namespace Ast
END_TL_NAMESPACE

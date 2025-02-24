#pragma once

#include "StrRef.h"

BEG_TL_NAMESPACE
namespace Ast {

/** 
    Class to write an Ast 

*/
class StringStore {
public:
    using     StrMap     = StrRef::Map;

    /**/      StringStore();

    StrRef string     ( const Str &val );

private:
    StrMap    str_map;
    PI        str_len;
};

} // namespace Ast
END_TL_NAMESPACE

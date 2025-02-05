#pragma once

#include "../../support/common_types.h"
#include <map>

BEG_TL_NAMESPACE
namespace Ast {

/** 
    Reference to a string to be written by an Ast::Writer
*/
class StringRef {
public:
    using    Map       = std::map<Str,PI>; ///< str to offset
    using    Ptr       = Map::iterator;
   
    operator StrView   () const { auto beg = ptr->first.begin() + off; return { beg, beg + len }; }
    bool     operator==( const StringRef &that ) const { return std::tie( ptr, off, len ) == std::tie( that.ptr, that.off, that.len ); }

    Ptr      ptr;      ///<
    PI       off;      ///<
    PI       len;      ///<
};

} // namespace Ast
END_TL_NAMESPACE

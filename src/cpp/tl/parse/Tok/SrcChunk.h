#pragma once

#include "../Ast/StrRef.h"

BEG_TL_NAMESPACE
namespace Tok {

/** 
*/
struct SrcChunk {
    PI             ref_count = 0; ///< scope size during declaration
    Ast::StrRef src_url;
    PI             src_off;
    char           beg[ 4 ]; ///<
};

} // namespace Tok
END_TL_NAMESPACE

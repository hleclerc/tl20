#pragma once

#include "CPtr.h"

BEG_TL_NAMESPACE
namespace Ast {

/** 
    Compact/continuous representation of an ast
*/
class CRepr {
public:
    CPtr<CVec<>> strings;
};

} // namespace Ast
END_TL_NAMESPACE

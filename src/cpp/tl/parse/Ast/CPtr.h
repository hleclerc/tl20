#pragma once

#include "../../support/common_types.h"

BEG_TL_NAMESPACE
namespace Ast {

/** pointer in a compact/continuous representation (CRepr)
*/
template<class T>
class CPtr {
public:
    const T*    operator->() const { return reinterpret_cast<const T *>( ptr + offset() ); }
    PI32        offset    () const { return *reinterpret_cast<const PI32 *>( ptr ); }
    operator    bool      () const { return offset(); }

    const char* ptr;
};

} // namespace Ast
END_TL_NAMESPACE

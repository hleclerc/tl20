#pragma once

#include "containers/CtType.h"

template<class T>
inline T conv( CtType<T>, const auto &value ) {
    return value;
}

#pragma once

#include "IsScalar.h"

BEG_TL_NAMESPACE

///
template<class T>
struct ItemTypeOf;

// scalar
template<IsScalar T>
struct ItemTypeOf<T> {
    using value = T;
};

END_TL_NAMESPACE

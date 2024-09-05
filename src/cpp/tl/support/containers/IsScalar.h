#pragma once

#include "TensorOrder.h"

BEG_TL_NAMESPACE

template<class T>
concept IsScalar = TensorOrder<T>::value == 0;

END_TL_NAMESPACE

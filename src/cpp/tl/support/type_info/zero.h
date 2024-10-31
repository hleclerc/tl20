#pragma once

#include "../containers/CtType.h"

BEG_TL_NAMESPACE

template<class T> T zero( CtType<T> ) { return 0; }

END_TL_NAMESPACE

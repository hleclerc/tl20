#pragma once

// #include "select_with_n_indices.h"
// #include "CtRange.h"
// #include <cstdlib>
// #include <limits>
#include "HeterogeneousVec.h"

BEG_TL_NAMESPACE

/// static vector ---------------------------------------------------------------------
#define DTP template<class... ItemVecs>
#define UTP HeterogeneousVec<ItemVecs...>

// DTP PI UTP::size() const {
//     return size_;
// }

#undef DTP
#undef UTP

END_TL_NAMESPACE

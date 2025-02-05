#pragma once

#include "../common_types.h"

BEG_TL_NAMESPACE
class StringStore;

/**
 */
class RefInStringStore {
public:
    StringStore* string_store;
    PI           off;
    PI           len;
};

END_TL_NAMESPACE

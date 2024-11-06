#pragma once

#include "../tl_namespace.h"
#include <type_traits>

BEG_TL_NAMESPACE

template<class T> 
struct IsTriviallyCopyable {
    static constexpr bool value = std::is_trivially_copyable_v<T>;
};

template<class T> 
concept TriviallyCopyable = IsTriviallyCopyable<T>::value;

END_TL_NAMESPACE

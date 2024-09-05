#pragma once

#include "../tl_namespace.h"
#include "PrimitiveCtInt.h"

BEG_TL_NAMESPACE

///
template<int... a>
struct PrimitiveCtIntList {
    static constexpr int size = sizeof...( a );
    auto display( auto &ds ) const { return ds.array( { ds.number( a )... } ); }
};

// max ----------------------------------------------------------------------------------------------------------
template<int ha,int... ta,int hb,int... tb,class ...IntsSoFar>
auto max_PrimitiveCtIntList( PrimitiveCtIntList<ha,ta...>, PrimitiveCtIntList<hb,tb...>, IntsSoFar... ints_so_far ) {
    constexpr int hm = ha >= hb ? ha : hb;
    return max_PrimitiveCtIntList( PrimitiveCtIntList<ta...>(), PrimitiveCtIntList<tb...>(), ints_so_far..., PrimitiveCtInt<hm>() );
}

template<int... a,class ...IntsSoFar>
auto max_PrimitiveCtIntList( PrimitiveCtIntList<a...>, PrimitiveCtIntList<>, IntsSoFar... ints_so_far ) {
    return PrimitiveCtIntList<IntsSoFar::value...,a...>();
}

template<int... b,class ...IntsSoFar>
auto max_PrimitiveCtIntList( PrimitiveCtIntList<>, PrimitiveCtIntList<b...>, IntsSoFar... ints_so_far ) {
    return PrimitiveCtIntList<IntsSoFar::value...,b...>();
}

template<class ...IntsSoFar>
auto max_PrimitiveCtIntList( PrimitiveCtIntList<>, PrimitiveCtIntList<>, IntsSoFar... ints_so_far ) {
    return PrimitiveCtIntList<IntsSoFar::value...>();
}

template<int... ta,int... tb>
auto max( PrimitiveCtIntList<ta...>, PrimitiveCtIntList<tb...> ) {
    return max_PrimitiveCtIntList( PrimitiveCtIntList<ta...>(), PrimitiveCtIntList<tb...>() );
}

END_TL_NAMESPACE

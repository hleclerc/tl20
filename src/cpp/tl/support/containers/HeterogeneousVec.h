#pragma once

#include "accessors/for_each_item.h"
#include <utility>
#include "Vec.h"
#include "tl/support/common_macros.h"

BEG_TL_NAMESPACE

template<class... ItemVecs>
class HeterogeneousVec;

/// basic sequence of Items with potentially static size and potential room for local data
/// Alignement is specified in bytes and can be = 0.
///
///
/// This specialization is for static vectors
template<class HeadVec,class... TailVecs>
class HeterogeneousVec<HeadVec,TailVecs...> {
public:
    using   Next            = HeterogeneousVec<TailVecs...>;
    using   Curr            = ItemTypeOf<HeadVec>::value;

    /**/    HeterogeneousVec( auto &&...values ) { ( operator<<( FORWARD( values ) ), ... ); }

    auto&   operator<<      ( const Curr &value ) { head_vec << value; return *this; }
    auto&   operator<<      ( Curr &&value ) { head_vec << std::move( value ); return *this; }
    auto&   operator<<      ( auto &&value ) { next << FORWARD( value ); return *this; }
   
    void    for_each_item   ( auto &&func ) const { ::for_each_item( head_vec, func ); next.for_each_item( FORWARD( func ) ); }
    PI      size            () const { return head_vec.size() + next.size(); }

    HeadVec head_vec;
    Next    next;
}; 

template<>
class HeterogeneousVec<> {
public:
    void    for_each_item( auto &&func ) const {}
    PI      size         () const { return 0; }
}; 

END_TL_NAMESPACE

#include "HeterogeneousVec.cxx" // IWYU pragma: export

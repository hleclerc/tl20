#pragma once

#include "../common_types.h"
#include "CtType.h"
#include "CtInt.h"

BEG_TL_NAMESPACE

/// ct known size version
template<class T,int size_=-1>
struct Span {
    constexpr          Span      ( const T *data ) : data_( const_cast<T *>( data ) ) {}
    constexpr          Span      ( T *data ) : data_( data ) {}

    constexpr auto     size      () const { return CtInt<size_>(); }

    constexpr const T& operator[]( PI index ) const { return data_[ index ]; }
    constexpr T&       operator[]( PI index ) { return data_[ index ]; }

    const T*           data      () const { return data_; }
    T*                 data      () { return data_; }

    const T*           begin     () const { return data_; }
    T*                 begin     () { return data_; }

    const T*           end       () const { return data_ + size_; }
    T*                 end       () { return data_ + size_; }

    auto               subspan   ( PI offset ) const { return Span<T>( data_ + offset, size_ - offset ); }

private:
    T*                 data_;
};

/// dynamic version
template<class T>
struct Span<T,-1> {
    constexpr          Span      ( const T *data, PI size ) : data_( const_cast<T *>( data ) ), size_( size ) {}
    T_i constexpr      Span      ( Span<T,i> data ) : data_( data.data() ), size_( data.size() ) {}
    constexpr          Span      () : data_( nullptr ), size_( 0 ) {}

    constexpr auto     size      () const { return size_; }

    constexpr const T& operator[]( PI index ) const { return data_[ index ]; }
    constexpr T&       operator[]( PI index ) { return data_[ index ]; }

    const T*           data      () const { return data_; }
    T*                 data      () { return data_; }

    const T*           begin     () const { return data_; }
    T*                 begin     () { return data_; }

    const T*           end       () const { return data_ + size_; }
    T*                 end       () { return data_ + size_; }

    auto               subspan   ( PI beg, PI end ) const { return Span<T>( data_ + beg, end - beg ); }
    auto               subspan   ( PI beg ) const { return Span<T>( data_ + beg, size_ - beg ); }

    bool               empty     () const { return size_ == 0; }

    void               resize    ( PI size ) { size_ = size; }

private:
    T*                 data_;
    PI                 size_;
};

// common functions
#define DTP template<class T,int s>
#define UTP Span<T,s>

DTP auto get_compilation_flags( auto &cn, CtType<UTP> ) { cn.add_inc_file( "vfs/containers/Span.h" ); }
DTP void for_each_template_arg( CtType<UTP>, auto &&f ) { f( CtType<T>() ); f( CtInt<s>() ); }
DTP auto template_type_name( CtType<UTP> ) { return "Vec"; }

#undef DTP
#undef UTP

END_TL_NAMESPACE

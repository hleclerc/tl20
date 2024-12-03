#pragma once

#include "../common_types.h"
#include "CtType.h"
#include "CtInt.h"

BEG_TL_NAMESPACE

/// ct known size version
template<class T,int size_=-1>
struct Span {
    constexpr          Span      ( T *data ) : _data( data ) {}

    constexpr auto     size      () const { return CtInt<size_>(); }

    constexpr const T& operator[]( PI index ) const { return _data[ index ]; }
    constexpr T&       operator[]( PI index ) { return _data[ index ]; }

    const T*           data      () const { return _data; }
    T*                 data      () { return _data; }

    const T*           begin     () const { return _data; }
    T*                 begin     () { return _data; }

    const T*           end       () const { return _data + size_; }
    T*                 end       () { return _data + size_; }

    auto               subspan   ( PI offset ) const { return Span<T>( _data + offset, size_ - offset ); }

private:
    T*                 _data;
};

/// dynamic version
template<class T>
struct Span<T,-1> {
    constexpr          Span      ( T *data, PI size ) : _data( data ), _size( size ) {}
    T_i constexpr      Span      ( Span<T,i> data ) : _data( data.data() ), _size( data.size() ) {}
    constexpr          Span      () : _data( nullptr ), _size( 0 ) {}

    constexpr auto     size      () const { return _size; }

    constexpr const T& operator[]( PI index ) const { return _data[ index ]; }
    constexpr T&       operator[]( PI index ) { return _data[ index ]; }

    const T*           data      () const { return _data; }
    T*                 data      () { return _data; }

    const T*           begin     () const { return _data; }
    T*                 begin     () { return _data; }

    const T*           end       () const { return _data + _size; }
    T*                 end       () { return _data + _size; }

    auto               subspan   ( PI beg, PI end ) const { return Span<T>( _data + beg, end - beg ); }
    auto               subspan   ( PI beg ) const { return Span<T>( _data + beg, _size - beg ); }

    bool               empty     () const { return _size == 0; }

    void               resize    ( PI size ) { _size = size; }

    const T&           front     () const { return _data[ 0 ]; }
    T&                 front     () { return _data[ 0 ]; }

    const T&           back      () const { return _data[ _size - 1 ]; }
    T&                 back      () { return _data[ _size - 1 ]; }


private:
    T*                 _data;
    PI                 _size;
};

// common functions
#define DTP template<class T,int s>
#define UTP Span<T,s>

DTP auto get_compilation_flags( auto &cn, CtType<UTP> ) { cn.add_inc_file( "tl/containers/Span.h" ); }
DTP void for_each_template_arg( CtType<UTP>, auto &&f ) { f( CtType<T>() ); f( CtInt<s>() ); }
DTP auto template_type_name( CtType<UTP> ) { return "Span"; }

#undef DTP
#undef UTP

END_TL_NAMESPACE

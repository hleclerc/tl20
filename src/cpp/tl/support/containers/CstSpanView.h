#pragma once

#include "../common_types.h"

BEG_TL_NAMESPACE

/**
 * @brief 
 * 
 * @tparam T item type
 */
template<class T>
class CstSpanView {
public:
    /**/               CstSpanView  ( const T *data, PI beg_index, PI end_index, PI global_size ) : _global_size( global_size ), _beg_index( beg_index ), _end_index( end_index ), _data( data ) {}
  
    constexpr auto     global_size  () const { return _global_size; }
    constexpr auto     local_size   () const { return _end_index - _beg_index; }
  
    constexpr const T& operator[]   ( PI global_index ) const { return _data[ global_index ]; }
   
    const T*           data         () const { return _data; }
   
    bool               locally_empty() const { return _beg_index == _end_index; }
    const T*           begin        () const { return _data + _beg_index; }
    const T*           end          () const { return _data + _end_index; }

    PI                 beg_index    () const { return _beg_index; }
    PI                 end_index    () const { return _end_index; }

private:
    PI                 _global_size;
    PI                 _beg_index;
    PI                 _end_index;
    const T*           _data;
};

// // common functions
// #define DTP template<class T,int s>
// #define UTP CstSpan<T,s>

// DTP auto get_compilation_flags( auto &cn, CtType<UTP> ) { cn.add_inc_file( "tl/containers/CstSpan.h" ); }
// DTP void for_each_template_arg( CtType<UTP>, auto &&f ) { f( CtType<T>() ); f( CtInt<s>() ); }
// DTP auto template_type_name( CtType<UTP> ) { return "CstSpan"; }

// #undef DTP
// #undef UTP

END_TL_NAMESPACE

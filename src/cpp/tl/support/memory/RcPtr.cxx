#pragma once

#include <utility>
#include "RcPtr.h"

template<class T>
RcPtr<T>::RcPtr() : data( nullptr ) {
}

template<class T>
RcPtr<T>::RcPtr( RcPtr &&obj ) : data( std::exchange( obj.data, nullptr ) ) {
}

template<class T>
RcPtr<T>::RcPtr( const RcPtr &obj ) : data( obj.data ) {
    inc_ref( data );
}

template<class T> template<class U>
RcPtr<T>::RcPtr( const RcPtr<U> &obj ) : data( obj.data ) {
    inc_ref( data );
}

template<class T> template<class U>
RcPtr<T>::RcPtr( RcPtr<U> &&obj ) : data( std::exchange( obj.data, nullptr ) ) {
}

template<class T> template<class U>
RcPtr<T>::RcPtr( U *obj ) : data( obj ) {
    inc_ref( data );
}

template<class T>
RcPtr<T>::~RcPtr() {
    dec_ref( data );
}

template<class T> T_U
RcPtr<T> RcPtr<T>::from_ptr( U *ptr ) {
    inc_ref( ptr );

    RcPtr res;
    res.data = ptr;
    return res;
}

template<class T>
RcPtr<T> &RcPtr<T>::operator=( const RcPtr &obj ) {
    inc_ref( obj.data );
    dec_ref( data );
    data = obj.data;
    return *this;
}

template<class T> template<class U>
RcPtr<T> &RcPtr<T>::operator=( const RcPtr<U> &obj ) {
    inc_ref( obj.data );
    dec_ref( data );
    data = obj.data;
    return *this;
}

template<class T>
RcPtr<T> &RcPtr<T>::operator=( RcPtr &&obj ) {
    dec_ref( data );
    data = std::exchange( obj.data, nullptr );
    return *this;
}

template<class T> template<class U>
RcPtr<T> &RcPtr<T>::operator=( RcPtr<U> &&obj ) {
    dec_ref( data );
    data = std::exchange( obj.data, nullptr );
    return *this;
}


template<class T>
RcPtr<T>::operator bool() const {
    return data;
}

template<class T>
void RcPtr<T>::clear() {
    dec_ref( std::exchange( data, nullptr ) );
}

template<class T>
bool RcPtr<T>::operator==( const RcPtr<T> &p ) const { return data == p.data; }

template<class T>
bool RcPtr<T>::operator!=( const RcPtr<T> &p ) const { return data != p.data; }

template<class T>
bool RcPtr<T>::operator< ( const RcPtr<T> &p ) const { return data <  p.data; }

template<class T>
bool RcPtr<T>::operator<=( const RcPtr<T> &p ) const { return data <= p.data; }

template<class T>
bool RcPtr<T>::operator> ( const RcPtr<T> &p ) const { return data >  p.data; }

template<class T>
bool RcPtr<T>::operator>=( const RcPtr<T> &p ) const { return data >= p.data; }

template<class T>
T *RcPtr<T>::get() const { return data; }

template<class T>
T *RcPtr<T>::operator->() const { return data; }

template<class T>
T &RcPtr<T>::operator*() const { return *data; }

template<class T>
void RcPtr<T>::inc_ref( T *data ) {
    if ( data )
        ++data->ref_count;
}

template<class T>
void RcPtr<T>::dec_ref( T *data ) {
    if ( data && --data->ref_count == 0 )
        delete data;
}

template<class T>
void RcPtr<T>::inc_ref() {
    inc_ref( data );
}

template<class T>
void RcPtr<T>::dec_ref() {
    dec_ref( data );
}

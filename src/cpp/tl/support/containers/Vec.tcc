#pragma once

#include "select_with_n_indices.h"
#include "CtRange.h"
#include <cstdlib>
#include <limits>
#include "Vec.h"

BEG_TL_NAMESPACE

/// static vector ---------------------------------------------------------------------
#define DTP template<class Item,int static_size>
#define UTP Vec<Item,static_size>

DTP UTP::Vec( FromInitFunctionOnIndex, auto &&func ) {
    for( PI index = 0; index < size(); ++index )
        func( data( index ), index );
}

DTP UTP::Vec( FromFunctionOnIndex, auto &&func ) {
    for( PI index = 0; index < size(); ++index )
        new ( data( index ) ) Item( func( index ) );
}

DTP T_is UTP::Vec( FromOperationOnItemsOf, auto &&functor, PrimitiveCtIntList<i...>, auto &&...lists ) {
    for( PI index = 0; index < size(); ++index )
        new ( data( index ) ) Item( functor( select_with_n_indices( lists, CtInt<i>(), index )... ) );
}

DTP UTP::Vec( FromItemValues, auto &&...values ) {
    PI index = 0;

    static_assert( sizeof...( values ) <= static_size );
    ( new ( data( index++ ) ) Item( FORWARD( values ) ), ... );

    while( index < static_size )
        new ( data( index++ ) ) Item;
}

DTP UTP::Vec( FromItemValue, auto &&...ctor_args ) {
    for( PI index = 0; index < size(); ++index )
        new ( data( index ) ) Item( ctor_args... );
}

DTP UTP::Vec( FromIterator, auto iter ) {
    for( PI index = 0; index < size(); ++index )
        new ( data( index ) ) Item( *( iter++ ) );
}

DTP UTP::Vec( FromUninit ) {
}

DTP T_T UTP::Vec( const std::initializer_list<T> &lst ) {
    auto iter = lst.begin();
    for( PI index = 0; index < std::min( PI( lst.size() ), PI( size() ) ); ++index )
        new ( data( index ) ) Item( *(iter++) );

    for( PI index = lst.size(); index < size(); ++index )
        new ( data( index ) ) Item;
}

DTP UTP::Vec( const HasSizeAndAccess auto &l ) {
    using namespace std;
    if constexpr( requires { l[ 0 ]; } ) {
        for( PI index = 0; index < min( size(), l.size() ); ++index )
            new ( data( index ) ) Item( l[ index ] );
    } else {
        PI index = 0;
        for( const auto &v : l ) {
            if ( index >= size() )
                break;
            new ( data( index++ ) ) Item( v );
        }
    }

    for( PI index = l.size(); index < size(); ++index )
        new ( data( index ) ) Item;
}

DTP UTP::Vec( const Vec &that ) {
    for( PI index = 0; index < size(); ++index )
        new ( data( index ) ) Item( that[ index ] );
}

DTP UTP::Vec( Vec &&that ) {
    for( PI index = 0; index < size(); ++index )
        new ( data( index ) ) Item( std::move( that[ index ] ) );
}

DTP UTP::Vec() {
    for( PI index = 0; index < size(); ++index )
        new ( data( index ) ) Item;
}

DTP UTP::~Vec() {
    for( PI i = static_size; i--; )
        data( i )->~Item();
}

DTP UTP &UTP::operator=( const Vec &that ) {
    for( PI i = 0; i < size(); ++i )
        operator[]( i ) = that[ i ];
    return *this;
}

DTP UTP &UTP::operator=( Vec &&that ) {
    for( PI i = 0; i < size(); ++i )
        operator[]( i ) = std::move( that[ i ] );
    return *this;
}

DTP const Item &UTP::operator[]( PI index ) const {
    return data()[ index ];
}

DTP Item &UTP::operator[]( PI index ) {
    return data()[ index ];
}

DTP const Item &UTP::operator()( PI index ) const {
    return data()[ index ];
}

DTP Item &UTP::operator()( PI index ) {
    return data()[ index ];
}

DTP const Item *UTP::data( PI index ) const {
    return data() + index;
}

DTP Item *UTP::data( PI index ) {
    return data() + index;
}

DTP const Item *UTP::data() const {
    return reinterpret_cast<const Item *>( data_ );
}

DTP Item *UTP::data() {
    return reinterpret_cast<Item *>( data_ );
}

DTP Vec<Item,static_size+1> UTP::with_pushed_value( auto&&...ctor_args ) const {
    Vec<Item,static_size+1> res = FromUninit();
    for( PI i = 0; i < static_size; ++i )
        new ( res.data() + i ) Item( operator[]( i ) );
    new ( res.data() + static_size ) Item( FORWARD( ctor_args )... );
    return res;
}

DTP T_i auto UTP::without_index( CtInt<i> index ) const {
    Vec<Item,static_size-1> res;
    CtRange<0,i>::for_each_item( [&]( int ind ) {
        res[ ind ] = operator[]( ind );
    } );
    CtRange<i,static_size-1>::for_each_item( [&]( int ind ) {
        res[ ind ] = operator[]( ind + 1 );
    } );
    return res;
}

DTP Vec<Item,static_size-1> UTP::without_index( PI index ) const {
    Vec<Item,static_size-1> res;
    for( PI i = 0; i < index; ++i )
        res[ i ] = operator[]( i );
    for( PI i = index; i < PI( static_size - 1 ); ++i )
        res[ i ] = operator[]( i + 1 );
    return res;
}

#undef DTP
#undef UTP

/// dynamic vector ---------------------------------------------------------------------
#define DTP template<class Item>
#define UTP Vec<Item,-1>

DTP UTP::Vec( FromSizeAndInitFunctionOnIndex, PI size, auto &&func ) : Vec( FromReservationSize(), size, size ) {
    for( PI index = 0; index < size; ++index )
        func( data_ + index, index );
}

DTP UTP::Vec( FromSizeAndItemValue, PI size, auto &&...ctor_args ) : Vec( FromReservationSize(), size, size ) {
    for( PI index = 0; index < size; ++index )
        new ( data_ + index ) Item( FORWARD( ctor_args )... );
}

DTP UTP::Vec( FromSizeAndIterator, PI size, auto iterator ) : Vec( FromReservationSize(), size, size ) {
    for( PI index = 0; index < size; ++index )
        new ( data_ + index ) Item( *( iterator++ ) );
}

DTP UTP::Vec( FromSize, PI size ) : Vec( FromReservationSize(), size, size ) {
    for( PI index = 0; index < size; ++index )
        new ( data_ + index ) Item;
}

DTP T_is UTP::Vec( FromOperationOnItemsOf, auto &&functor, PrimitiveCtIntList<i...>, auto &&...lists ) {
    // compute size
    PI size = std::numeric_limits<PI>::max();
    auto get_size = [&]( auto nb_to_take, const auto &list ) {
        if constexpr ( nb_to_take )
            size = std::min( size, list.size() );
    };
    ( get_size( CtInt<i>(), lists ), ... );

    // reserve
    data_ = allocate( size );
    size_ = size;
    capa_ = size;

    // fill
    for( PI index = 0; index < size; ++index )
        new ( data_ + index ) Item( functor( select_with_n_indices( lists, CtInt<i>(), index )... ) );
}

DTP UTP::Vec( FromReservationSize, PI capa, PI raw_size ) {
    data_ = allocate( capa, CtInt<1>() );
    size_ = raw_size;
    capa_ = capa;
}

DTP UTP::Vec( FromItemValues, auto &&...values ) : Vec( FromReservationSize(), sizeof...( values ), sizeof...( values ) ) {
    PI index = 0;
    ( new ( data_ + index++ ) Item( FORWARD( values ) ), ... );
}

DTP UTP::Vec( const std::initializer_list<Item> &l ) : Vec( FromReservationSize(), l.size(), l.size() ) {
    PI index = 0;
    for( const Item &v : l )
        new ( data_ + index++ ) Item( v );
}

DTP UTP::Vec( const HasSizeAndAccess auto &l ) : Vec( FromReservationSize(), l.size(), l.size() ) {
    if constexpr( requires { l[ 0 ]; } ) {
        for( PI index = 0; index < l.size(); ++index )
            new ( data_ + index ) Item( l[ index ] );
    } else {
        PI index = 0;
        for( const auto &v : l )
            new ( data_ + index++ ) Item( v );
    }
}

DTP UTP::Vec( const Vec &that ) : Vec( FromReservationSize(), that.size(), that.size() ) {
    for( PI index = 0; index < that.size(); ++index )
        new ( data_ + index ) Item( that[ index ] );
}

DTP UTP::Vec( Vec &&that ) {
    data_ = std::exchange( that.data_, nullptr );
    size_ = std::exchange( that.size_, 0 );
    capa_ = std::exchange( that.capa_, 0 );
}

DTP UTP::Vec() : Vec( FromReservationSize(), 0, 0 ) {
}

DTP UTP::~Vec() {
    if ( capa_ ) {
        for( PI i = size(); i--; )
            data( i )->~Item();
        std::free( data_ );
    }
}

DTP UTP &UTP::operator=( const Vec &that ) {
    // need more room ?
    if ( capa_ < that.size() ) {
        if ( capa_ ) {
            for( PI i = size(); i--; )
                data( i )->~Item();
            std::free( data_ );
        } else
            capa_ = 1;

        while ( capa_ < that.size() )
            capa_ *= 2;

        data_ = allocate( capa_, CtInt<1>() );
        size_ = that.size_;
        for( PI i = 0; i < that.size_; ++i )
            new ( data_ + i ) Item( that[ i ] );
        return *this;
    }

    // else, copy in place
    for( PI i = 0; i < std::min( size_, that.size_ ); ++i )
        *data( i ) = that[ i ];
    for( ; size_ < that.size_; ++size_ )
        new ( data_ + size_ ) Item( that[ size_ ] );
    for( ; size_ > that.size_; )
        data( --size_ )->~Item();

    return *this;
}

DTP UTP &UTP::operator=( Vec &&that ) {
    if ( capa_ ) {
        for( PI i = size(); i--; )
            data( i )->~Item();
        std::free( data_ );
    }
    data_ = std::exchange( that.data_, nullptr );
    size_ = std::exchange( that.size_, 0 );
    capa_ = std::exchange( that.capa_, 0 );
    return *this;
}

DTP const Item &UTP::operator[]( PI index ) const {
    return data_[ index ];
}

DTP Item &UTP::operator[]( PI index ) {
    return data_[ index ];
}

DTP const Item *UTP::data( PI index ) const {
    return data_ + index;
}

DTP Item *UTP::data( PI index ) {
    return data_ + index;
}

DTP const Item *UTP::data() const {
    return data_;
}

DTP Item *UTP::data() {
    return data_;
}

DTP PI UTP::size() const {
    return size_;
}

DTP Item *UTP::push_back_unique( auto &&value ) {
    for( PI i = 0; i < size(); ++i )
        if ( operator[]( i ) == value )
            return &operator[]( i );
    return push_back( FORWARD( value ) );
}

DTP Item UTP::pop_back_val() {
    PI pos = --size_;
    Item res = std::move( data_[ pos ] );
    data_[ pos ].~Item();
    return res;
}

DTP Item *UTP::push_back_br( auto&&...args ) {
    reserve( size_ + 1 );
    return new ( data_ + size_++ ) Item{ FORWARD( args )... };
}

DTP PI UTP::push_back_ind( auto&&...args ) {
    PI res = size();
    push_back( FORWARD( args )... );
    return res;
}

DTP Item *UTP::push_back( auto&&...args ) {
    reserve( size_ + 1 );
    return new ( data_ + size_++ ) Item( FORWARD( args )... );
}

DTP void UTP::resize( PI size, auto&&...ctor_args ) {
    aligned_resize( size, CtInt<1>(), FORWARD( ctor_args )... );
}

DTP void UTP::append( auto &&that ) {
    bool move = std::is_rvalue_reference_v<decltype(that)>;
    // if constexpr( ... )
    reserve( size_ + that.size() );
    if ( move )
        for( PI i = 0; i < that.size(); ++i )
            new ( data_ + size_++ ) Item( std::move( that[ i ] ) );
    else
        for( PI i = 0; i < that.size(); ++i )
            new ( data_ + size_++ ) Item( that[ i ] );
}

DTP void UTP::fill( auto&&...ctor_args ) {
    for( PI i = 0; i < size(); ++i )
        operator[]( i ) = Item( FORWARD( ctor_args )... );
}


DTP void UTP::clear() {
    while( size_ )
        data_[ --size_ ].~Item();
}

DTP void UTP::aligned_resize_woc( PI size, auto alig, auto&&...ctor_args ) {
    aligned_reserve_woc( size, alig );

    while( size_ > size )
        data_[ --size_ ].~Item();

    while( size_ < size )
        new ( data_ + size_++ ) Item( FORWARD( ctor_args )... );
}

DTP void UTP::aligned_resize( PI size, auto alig, auto&&...ctor_args ) {
    aligned_reserve( size, alig );

    while( size_ > size )
        data_[ --size_ ].~Item();

    while( size_ < size )
        new ( data_ + size_++ ) Item( FORWARD( ctor_args )... );
}

DTP void UTP::aligned_reserve_woc( PI tgt_capa, auto alig ) {
    if ( capa_ >= tgt_capa )
        return;

    PI new_capa = capa_ ? capa_ : 1;
    while ( new_capa < tgt_capa )
        new_capa *= 2;

    Item *new_data = allocate( new_capa, alig );
    for( PI i = 0; i < size_; ++i )
        new ( new_data + i ) Item;
    for( PI i = size_; i--; )
        data_[ i ].~Item();

    if ( capa_ )
        std::free( data_ );

    capa_ = new_capa;
    data_ = new_data;
}

DTP void UTP::aligned_reserve( PI tgt_capa, auto alig ) {
    if ( capa_ >= tgt_capa )
        return;

    PI new_capa = capa_ ? capa_ : 1;
    while ( new_capa < tgt_capa )
        new_capa *= 2;

    Item *new_data = allocate( new_capa, alig );
    for( PI i = 0; i < size_; ++i )
        new ( new_data + i ) Item( std::move( data_[ i ] ) );
    for( PI i = size_; i--; )
        data_[ i ].~Item();

    if ( capa_ )
        std::free( data_ );

    capa_ = new_capa;
    data_ = new_data;
}


DTP void UTP::reserve( PI tgt_capa ) {
    aligned_reserve( tgt_capa, CtInt<1>() );
}

DTP void UTP::remove( PI beg, PI len ) {
    if ( len == 0 )
        return;
    const PI new_size = size_ - len;
    for( PI i = beg; i < new_size; ++i )
        data_[ i ] = std::move( data_[ len + i ] );
    for( PI i = new_size; i < size_; ++i )
        data_[ i ].~Item();
    size_ = new_size;
}

DTP void UTP::copy_data_to( void *data ) const {
    for( PI i = 0; i < size_; ++i )
        new ( reinterpret_cast<Item *>( data ) + i ) Item( data_[ i ] );
}

DTP Item *UTP::allocate( PI nb_items, auto alig ) {
    if ( nb_items == 0 )
        return nullptr;

    // 8ul because std::aligned_alloc seems to return bad results if al if < 8...
    constexpr PI al = std::max( PI( alig ), alignof( Item ) );
    if constexpr ( al > 8ul )
        return reinterpret_cast<Item *>( std::aligned_alloc( alig, sizeof( Item ) * nb_items ) );

    return reinterpret_cast<Item *>( std::malloc( sizeof( Item ) * nb_items ) );
}

DTP UTP UTP::range( Item end ) {
    return { FromSizeAndInitFunctionOnIndex{}, end, []( Item *item, PI index ) { new ( item ) Item( index ); } };
}

#undef DTP
#undef UTP

END_TL_NAMESPACE

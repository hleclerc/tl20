#pragma once

#include "Span.h"

BEG_TL_NAMESPACE

/// basic sequence of Items with potentially static size and potential room for local data
/// Alignement is specified in bytes and can be = 0.
///
///
/// This specialization is for static vectors
template<class Item,int max_size>
class SmallVec : public WithDefaultOperators {
public:
    // static constexpr PI ct_size          = static_size;

    // // static auto   with_item_type   ( auto item_type ) { return CtType< Vec<typename VALUE_IN_DECAYED_TYPE_OF(item_type),static_size> >{}; }

    // /**/             SmallVec         ( FromInitFunctionOnIndex, auto &&func );
    // T_is             SmallVec         ( FromOperationOnItemsOf, auto &&functor, PrimitiveCtIntList<i...>, auto &&...lists );
    // /**/             SmallVec         ( FromItemValues, auto &&...values );
    // /**/             SmallVec         ( FromItemValue, auto &&...ctor_args );
    // /**/             SmallVec         ( FromIterator, auto iter );
    // /**/             SmallVec         ( FromUninit );
    // T_T              SmallVec         ( const std::initializer_list<T> &lst );
    // /**/             SmallVec         ( const HasSizeAndAccess auto &l );
    // /**/             SmallVec         ( const Vec &that );
    // /**/             SmallVec         ( Vec && );
    /**/                SmallVec         () : size_( 0 ) {}
    /**/               ~SmallVec         () { for( PI i = size_; i--; ) data( i )->~Item(); }

    // Vec&             operator=        ( const Vec & );
    // Vec&             operator=        ( Vec && );

    operator            Span<Item>       () const { return { data(), size() }; }

    const Item&         operator[]       ( PI index ) const { return data()[ index ]; }
    Item&               operator[]       ( PI index ) { return data()[ index ]; }
    const Item&         operator()       ( PI index ) const { return data()[ index ]; }
    Item&               operator()       ( PI index ) { return data()[ index ]; }
    PI                  size_tot         () const { return size(); }
    const Item*         begin            () const { return data(); }
    Item*               begin            () { return data(); }
    const Item*         data             ( PI index ) const { return data() + index; }
    Item*               data             ( PI index ) { return data() + index; }
    const Item*         data             () const { return reinterpret_cast<const Item *>( data_ ); }
    Item*               data             () { return reinterpret_cast<Item *>( data_ ); }
    const Item&         back             () const { return operator[]( size() - 1 ); }
    Item&               back             () { return operator[]( size() - 1 ); }
    const Item*         end              () const { return begin() + size(); }
    Item*               end              () { return begin() + size(); }

    PI                  size             ( PI d ) const { return size_; }
    PI                  size             () const { return size_; }

    Item*               push_back_br     ( auto&&...args ); ///< push_back with Item{ FORWARD( args )... }
    Item*               push_back        ( auto&&...args ); ///< push_back with Item( FORWARD( args )... )

    SmallVec&           operator<<       ( auto &&value ) { push_back( FORWARD( value) ); return *this; }

private:
    static constexpr PI nbch             = max_size * sizeof( Item );

    char                data_            [ nbch ]; ///<
    PI                  size_;
};


// // --------------------------------------------------------------------------------------------------------------------------------------------
#define DTP template<class Item,int max_size>
#define UTP SmallVec<Item,max_size>

DTP Item *UTP::push_back_br( auto&&...args ) {
    return new ( data( size_++ ) ) Item{ FORWARD( args )... };
}

DTP Item *UTP::push_back( auto&&...args ) {
    return new ( data( size_++ ) ) Item( FORWARD( args )... );
}

#undef DTP
#undef UTP

END_TL_NAMESPACE

// #include "Vec.tcc" // IWYU pragma: export

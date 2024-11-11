#pragma once

#include "../common_ctor_selectors.h"
#include "../common_concepts.h"
#include "../common_types.h"

#include "../type_info/TriviallyCopiable.h"
#include "../type_info/zero.h"

#include "WithDefaultOperators.h"
#include "CstSpan.h"
// #include <type_traits>

BEG_TL_NAMESPACE

/// basic sequence of Items with potentially static size and potential room for local data
/// Alignement is specified in bytes and can be = 0.
///
///
/// This specialization is for static vectors
template<class Item,int static_size=-1>
class Vec : public WithDefaultOperators {
public:   
    static constexpr PI ct_size              = static_size;
    using               value_type           = Item;
   
    // static auto      with_item_type       ( auto item_type ) { return CtType< Vec<typename VALUE_IN_DECAYED_TYPE_OF(item_type),static_size> >{}; }
   
    /**/                Vec                  ( FromInitFunctionOnIndex, auto &&func );
    T_is                Vec                  ( FromOperationOnItemsOf, auto &&functor, PrimitiveCtIntList<i...>, auto &&...lists );
    /**/                Vec                  ( FromFunctionOnIndex, auto &&func );
    /**/                Vec                  ( FromItemValues, auto &&...values );
    /**/                Vec                  ( FromItemValue, auto &&...ctor_args );
    /**/                Vec                  ( FromIterator, auto iter );
    /**/                Vec                  ( FromUninit );
    T_T                 Vec                  ( const std::initializer_list<T> &lst );
    /**/                Vec                  ( const HasSizeAndAccess auto &l );
    /**/                Vec                  ( const Vec &that );
    /**/                Vec                  ( Vec && );
    /**/                Vec                  ();
    /**/               ~Vec                  ();
   
    Vec&                operator=            ( const Vec & );
    Vec&                operator=            ( Vec && );

    operator            CstSpan<Item,ct_size>() const { return { data() }; }
    operator            CstSpan<Item>        () const { return { data(), size() }; }

    operator            Span<Item,ct_size>   () { return { data() }; }
    operator            Span<Item>           () { return { data(), size() }; }
   
    static Vec          zeros                () { return { FromItemValue(), zero( CtType<Item>() ) }; }
    static Vec          ones                 () { return { FromItemValue(), 1 }; }
   
    T_ij Vec<Item,j-i>  slice                ( CtInt<i> beg, CtInt<j> end ) const { return { FromIterator(), data() + i }; }
    T_ij Vec<Item,j-i>  slice                () const { return slice( CtInt<i>(), CtInt<j>() ); }
    Vec<Item>           slice                ( PI beg, PI end ) const { return { FromSizeAndIterator(), end - beg, data() + beg }; }
   
    const Item&         operator[]           ( PI index ) const;
    Item&               operator[]           ( PI index );
    const Item&         operator()           ( PI index ) const;
    Item&               operator()           ( PI index );
    bool                contains             ( const auto &v ) const { for( const auto &r : *this ) if ( r == v ) return true; return false; }
    PI                  size_tot             () const { return size(); }
    const Item*         begin                () const { return data(); }
    Item*               begin                () { return data(); }
    const Item*         data                 ( PI index ) const;
    Item*               data                 ( PI index );
    const Item*         data                 () const;
    Item*               data                 ();
    const Item&         back                 () const { return operator[]( size() - 1 ); }
    Item&               back                 () { return operator[]( size() - 1 ); }
    const Item*         end                  () const { return begin() + size(); }
    Item*               end                  () { return begin() + size(); }

    CtInt<static_size>  size                 ( PI d ) const { return {}; }
    CtInt<static_size>  size                 () const { return {}; }
   
    Vec&                operator+=           ( auto &&value ) { *this = *this + FORWARD( value ); return *this; }
    Vec&                operator-=           ( auto &&value ) { *this = *this - FORWARD( value ); return *this; }
    Vec&                operator*=           ( auto &&value ) { *this = *this * FORWARD( value ); return *this; }
    Vec&                operator/=           ( auto &&value ) { *this = *this / FORWARD( value ); return *this; }
   
    Vec                 operator-            () const { return { FromInitFunctionOnIndex(), [&]( Item *r, PI i ) { new ( r ) Item( - operator[]( i ) ); } }; }
   
    auto                with_pushed_value    ( auto&&...ctor_args ) const -> Vec<Item,static_size+1>;
    T_i auto            without_index        ( CtInt<i> index ) const;
    auto                without_index        ( PI index ) const -> Vec<Item,static_size-1>;
   
    static constexpr PI nbch                 = static_size * sizeof( Item );
    char                data_                [ nbch ]; ///<
};

// dynamic size, items fully on the heap
template<class Item>
class Vec<Item,-1> : public WithDefaultOperators {
public:
    using               value_type         = Item;
     
    /**/                Vec                ( FromSizeAndInitFunctionOnIndex, PI size, auto &&func );
    T_is                Vec                ( FromOperationOnItemsOf, auto &&functor, PrimitiveCtIntList<i...>, auto &&...lists );
    /**/                Vec                ( FromSizeAndItemValue, PI size, auto &&...ctor_args );
    /**/                Vec                ( FromSizeAndIterator, PI size, auto iterator );
    /**/                Vec                ( FromReservationSize, PI capa, PI raw_size = 0 );
    /**/                Vec                ( FromItemValues, auto &&...values );
    /**/                Vec                ( FromSize, PI size );
    /**/                Vec                ( const std::initializer_list<Item> &l );
    /**/                Vec                ( const HasSizeAndAccess auto &l );
    /**/                Vec                ( const Vec & );
    /**/                Vec                ( Vec && );
    /**/                Vec                ();
    /**/               ~Vec                ();
 
    operator            CstSpan<Item>      () const { return { data(), size() }; }
    operator            Span<Item>         () { return { data(), size() }; }
 
    static Vec          range              ( Item end );
 
    Vec&                operator=          ( const Vec &that );
    Vec&                operator=          ( Vec &&that );
 
    Vec&                operator<<         ( auto &&value ) { push_back( FORWARD( value) ); return *this; }
 
    Vec&                operator+=         ( auto &&value ) { *this = *this + FORWARD( value ); return *this; }
    Vec&                operator-=         ( auto &&value ) { *this = *this - FORWARD( value ); return *this; }
    Vec&                operator*=         ( auto &&value ) { *this = *this * FORWARD( value ); return *this; }
    Vec&                operator/=         ( auto &&value ) { *this = *this / FORWARD( value ); return *this; }
 
    const Item&         operator[]         ( PI index ) const;
    Item&               operator[]         ( PI index );
    PI                  size_tot           () const { return size(); }
    const Item*         begin              () const { return data(); }
    Item*               begin              () { return data(); }
    const Item&         front              () const { return operator[]( 0 ); }
    Item&               front              () { return operator[]( 0 ); }
    const Item*         data               ( PI index ) const;
    Item*               data               ( PI index );
    const Item*         data               () const;
    Item*               data               ();
    const Item&         back               () const { return operator[]( size() - 1 ); }
    Item&               back               () { return operator[]( size() - 1 ); }
    const Item*         end                () const { return begin() + size(); }
    Item*               end                () { return begin() + size(); }
 
    bool                contains           ( const auto &v ) const { for( const auto &r : *this ) if ( r == v ) return true; return false; }
    bool                empty              () const { return size_ == 0; }
    PI                  size               ( PI d ) const { return size(); }
    PI                  size               () const;
 
    Item*               push_back_unique   ( auto &&value );
    PI                  push_back_ind      ( auto&&...args ); ///< push_back with Item( FORWARD( args )... ) and return index of the new item
    Item*               push_back_br       ( auto&&...args ); ///< push_back with Item{ FORWARD( args )... }
    Item*               push_back          ( auto&&...args ); ///< push_back with Item( FORWARD( args )... )
 
    void                append             ( auto &&that );
 
    Item                pop_back_val       ();
    void                remove             ( PI beg, PI len );
    void                clear              ();

    void                aligned_reserve_woc( PI capa, auto alig ); ///< reserve without copy
    void                aligned_resize_woc ( PI size, auto alig, auto&&...ctor_args ); ///< resize without copy
    void                aligned_reserve    ( PI capa, auto alig );
    void                aligned_resize     ( PI size, auto alig, auto&&...ctor_args );
 
    void                reserve            ( PI capa );
    void                resize             ( PI size, auto&&...ctor_args );
    void                fill               ( auto&&...ctor_args ); ///<
 
    void                copy_data_to       ( void *data ) const;
 
    void                set_item           ( PI index, auto &&value ) { operator[]( index ) = value; }
    const Item&         get_item           ( const auto &index ) const { return operator[]( index ); }
 
    static Item*        allocate           ( PI nb_items, auto alig );
 
    Item*               data_;             ///<
    PI                  size_;             ///<
    PI                  capa_;             ///<
}; 
 
// // --------------------------------------------------------------------------------------------------------------------------------------------
#define DTP template<class Item,int static_size>
#define UTP Vec<Item,static_size>

// DTP auto           get_compilation_flags( auto &cn, CtType<UTP> ) { cn.add_inc_file( "vfs/containers/Vec.h" ); }
// DTP void           for_each_template_arg( CtType<UTP>, auto &&f ) { f( CtType<Item>() ); f( CtInt<static_size>() ); }
// DTP auto           template_type_name   ( CtType<UTP> ) { return "Vec"; }
// DTP constexpr auto tensor_order         ( CtType<UTP> ) { return CtInt<1>(); }

DTP struct StaticSizesOf<UTP> { using value = PrimitiveCtIntList<static_size>; };
DTP requires ( static_size >= 0 ) struct StaticSizeOf<UTP> { static constexpr PI value = static_size; };
DTP struct TensorOrder<UTP> { enum { value = 1 }; };
DTP struct ItemTypeOf<UTP> { using value = Item; };

DTP UTP zero( CtType<UTP> ) { return UTP::zeros(); }

template<class T,int i> 
struct IsTriviallyCopyable<Vec<T,i>> {
    static constexpr bool value = i >= 0 && TriviallyCopyable<T>;
};

// // DTP constexpr auto ct_sizes_of( CtType<UTP> ) { return CtIntList<static_size>(); }
// // DTP auto memory_of( const UTP &a ) { return Memory_Cpu(); }

// // DTP constexpr auto block_types_of( CtType<UTP> ) { return CtTypeList<Vec<UTP,1>>(); }

// Ti auto VecType_for( auto item_type, CtInt<i> ) { return CtType<Vec<typename VALUE_IN_DECAYED_TYPE_OF( item_type ),i>>(); }
// auto VecType_for( auto item_type, PI ) { return CtType<Vec<typename VALUE_IN_DECAYED_TYPE_OF( item_type )>>(); }

#undef DTP
#undef UTP

template<class ItemType,int static_size>
struct ArrayTypeFor<ItemType,PrimitiveCtIntList<static_size>,1> {
    using value = Vec<ItemType,static_size>;
};

/// return a vector containing func( input( i ) )
auto map_vec( auto &&input, auto &&func ) {
    if constexpr ( requires { input.begin(); } ) {
        using TR = DECAYED_TYPE_OF( func( *input.begin() ) );
        auto iter = input.begin();
        if constexpr ( requires { StaticSizeOf<DECAYED_TYPE_OF( input )>::value; } ) {
            using R = Vec<TR,StaticSizeOf<DECAYED_TYPE_OF( input )>::value>;
            return R{ FromInitFunctionOnIndex(), [&]( TR *v, PI i ) {
                new ( v ) TR( func( *( iter++ ) ) );
            } };
        } else {
            using R = Vec<TR>;
            return R{ FromSizeAndInitFunctionOnIndex(), PI( input.size() ), [&]( TR *v, PI i ) {
                new ( v ) TR( func( *( iter++ ) ) );
            } };
        }
    } else {
        using TR = DECAYED_TYPE_OF( func( input[ 0 ] ) );
        if constexpr ( requires { StaticSizeOf<DECAYED_TYPE_OF( input )>::value; } ) {
            using R = Vec<TR,StaticSizeOf<DECAYED_TYPE_OF( input )>::value>;
            return R{ FromInitFunctionOnIndex(), [&]( TR *v, PI i ) {
                new ( v ) TR( func( input[ i ] ) );
            } };
        } else {
            using R = Vec<TR>;
            return R{ FromSizeAndInitFunctionOnIndex(), PI( input.size() ), [&]( TR *v, PI i ) {
                new ( v ) TR( func( input[ i ] ) );
            } };
        }
    }
}

END_TL_NAMESPACE

#include "Vec.tcc" // IWYU pragma: export

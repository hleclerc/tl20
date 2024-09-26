#pragma once

#include "../STATIC_ASSERT_IN_IF_CONSTEXPR.h"
#include "../common_types.h"

#include "for_each_template_arg.h"
#include "template_type_name.h"

BEG_TL_NAMESPACE

// qualifiers
T_TI inline auto type_name( CtType<const T(&)[i]> ) { return type_name( CtType<const T *>() ); } // send the size ??
T_TI inline auto type_name( CtType<T(&)[i]> ) { return type_name( CtType<T *>() ); } // send the size ??

T_TI inline auto type_name( CtType<const T[i]> ) { return type_name( CtType<const T *>() ); } // send the size ??
T_TI inline auto type_name( CtType<T[i]> ) { return type_name( CtType<T *>() ); } // send the size ??

T_T  inline auto type_name( CtType<const T> ) { return Str( "const " ) + type_name( CtType<T>() ); }
T_T  inline auto type_name( CtType<T &&> ) { return Str( type_name( CtType<T>() ) ) + " &&"; }
T_T  inline auto type_name( CtType<T &> ) { return Str( type_name( CtType<T>() ) ) + " &"; }
T_T  inline auto type_name( CtType<T *> ) { return Str( type_name( CtType<T>() ) ) + " *"; }

// common types
#define DECL_TYPE_NAME( NAME ) inline auto type_name( CtType<NAME> ) { return #NAME; }
DECL_TYPE_NAME( PI64 );
DECL_TYPE_NAME( PI32 );
DECL_TYPE_NAME( PI16 );
DECL_TYPE_NAME( PI8  );

DECL_TYPE_NAME( SI64 );
DECL_TYPE_NAME( SI32 );
DECL_TYPE_NAME( SI16 );
DECL_TYPE_NAME( SI8  );

DECL_TYPE_NAME( Bool );
DECL_TYPE_NAME( Byte );

DECL_TYPE_NAME( FP64 );
DECL_TYPE_NAME( FP32 );

DECL_TYPE_NAME( char );

DECL_TYPE_NAME( void );

DECL_TYPE_NAME( Str  );
#undef DECL_DECL_TYPE_NAME

// special cases
template<class T,class... A> Str type_name( CtType<std::function<T(A...)>> ) {
    Str res;
    ( ( res += ( res.empty() ? "" : "," ) + type_name( CtType<A>() ) ), ... );
    return Str( "std::function<" ) + type_name( CtType<T>() ) + "(" + res + ")>";
}

// def for CtType
T_T void CtType<T>::display( auto &ds ) { ds << type_name( CtType<T>() ); }

// generic version
T_T Str type_name( CtType<T> ) {
    if constexpr( requires { T::type_name(); } ) {
        return T::type_name();
    } else if constexpr( requires { template_type_name( CtType<T>() ); } ) {
        Str res = template_type_name( CtType<T>() );
        if constexpr( requires { for_each_template_arg( CtType<T>(), []( auto ) {} ); } ) {
            res += "<";
            PI nb_template_args = 0;
            for_each_template_arg( CtType<T>(), [&]( auto n ) {
                if ( nb_template_args++ )
                    res += ",";
                res += n.to_string();
            } );
            res += ">";
        }
        return res;
    } else
        STATIC_ASSERT_WITH_RETURN_IN_IF_CONSTEXPR( "", 0, "found no way to get type_name" );
}

T_T auto CtType<T>::to_string() { return tyne_name( CtType<T>() ); }

// shortcut type_name<T>()
T_T Str type_name() { return type_name( CtType<T>() ); }

END_TL_NAMESPACE

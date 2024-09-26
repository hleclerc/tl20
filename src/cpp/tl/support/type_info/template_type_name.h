#pragma once

#include "../containers/CtTypeList.h"
#include <functional>

BEG_TL_NAMESPACE

/// common types
T_TA auto template_type_name( CtType<std::function<T(A...)>> ) { return "std::function"; }

#define DECL_BASE_TYPE_INFO_UV( NAME ) template<class U,class V> auto template_type_name( CtType<NAME<U,V>> ) { return #NAME; }
#define DECL_BASE_TYPE_INFO_A_( NAME ) template<class... A> auto template_type_name( CtType<NAME<A...>> ) { return #NAME; }
#define DECL_BASE_TYPE_INFO_T_( NAME ) template<class T> auto template_type_name( CtType<NAME<T>> ) { return #NAME; }

DECL_BASE_TYPE_INFO_T_( std::initializer_list );
DECL_BASE_TYPE_INFO_A_( CtTypeList            );
DECL_BASE_TYPE_INFO_T_( CtType                );

// DECL_BASE_TYPE_INFO_T_( UniquePtr          );
// DECL_BASE_TYPE_INFO_A_( Tup                );
// DECL_BASE_TYPE_INFO_UV( Pair               );

#undef DECL_BASE_TYPE_INFO_UV
#undef DECL_BASE_TYPE_INFO_A_
#undef DECL_BASE_TYPE_INFO_T_

// as method
T_U auto template_type_name( CtType<U> ) -> decltype( U::template_type_name() ) { return U::template_type_name(); }

// shortcut
T_U auto template_type_name() { return template_type_name( CtType<U>() ); }

END_TL_NAMESPACE

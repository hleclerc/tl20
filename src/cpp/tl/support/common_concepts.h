#pragma once

#include "common_macros.h"
#include <string_view>

BEG_TL_NAMESPACE

T_T concept HasSizeAndAccess = requires( const T &t ) { t.size(); } && ( requires( const T &t ) { t.begin(); } || requires( const T &t ) { t[ 0 ]; } );
T_T concept HasSizeAndSelect = requires( const T &t ) { t.size(); t[ 0 ]; };
T_T concept HasBeginAndEnd   = requires( const T &t ) { t.begin(); t.end(); };
T_T concept IteratorLike     = requires( T &t ) { *( t++ ); };
T_T concept FunctionLike     = std::is_function_v<std::decay_t<T>>;
T_T concept ScalarLike       = requires( const T &t ) { t * t; };
T_T concept ListLike         = HasSizeAndSelect<T> || HasBeginAndEnd<T>;
T_T concept StrLike          = std::is_convertible_v<T,std::string_view>;

END_TL_NAMESPACE

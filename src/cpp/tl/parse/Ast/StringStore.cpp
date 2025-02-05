#include "StringStore.h"

BEG_TL_NAMESPACE
namespace Ast {

StringStore::StringStore() {
    str_len = 0;
}

StringRef StringStore::string( const Str &val ) {
    auto iter = str_map.find( val );
    if ( iter == str_map.end() )
        iter = str_map.insert( iter, { val, std::exchange( str_len, str_len + val.size() ) } );
    return { iter, 0, val.size() };
}

} // namespace Ast
END_TL_NAMESPACE

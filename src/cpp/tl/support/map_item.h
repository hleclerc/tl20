#pragma once

#include "tl_namespace.h"

BEG_TL_NAMESPACE

template<class Map,class Key,class Func>
auto &map_item( Map &map, const Key &key, Func &&func ) {
    auto iter = map.find( key );
    if ( iter == map.end() )
        iter = map.insert( iter, { key, func() } );
    return iter->second;
}

template<class Map,class Key,class Func>
auto map_item_cst( const Map &map, const Key &key, Func &&func ) {
    auto iter = map.find( key );
    if ( iter != map.end() )
        return iter->second;
    return func();
}

END_TL_NAMESPACE

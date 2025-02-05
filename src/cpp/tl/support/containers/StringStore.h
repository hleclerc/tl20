#pragma once

#include "../common_types.h"

BEG_TL_NAMESPACE
class StringStore;

/**
 */
class RefInStringStore {
public:
    bool         operator==( const RefInStringStore &that ) const;
    operator     StrView   () const;

    StringStore* string_store;
    PI           off;
    PI           len;
};

/**
 */
class StringStore {
public:
    /**/    StringStore  () : current_ptr( nullptr ), ending_ptr( nullptr ), last_frame( nullptr ) {}

    StrView operator()   ( const RefInStringStore &ref ) const {}

private:
    struct  Frame        { Frame *prev_frame; char *ending_ptr; char content[ 8 ]; };
     
    char*   current_ptr; ///<
    char*   ending_ptr;  ///<
    Frame*  last_frame;  ///<
};

RefInStringStore::operator StrView() const { 
    auto beg = string_store->first.begin() + off; 
    return { beg, beg + len };
}
bool RefInStringStore::operator==( const RefInStringStore &that ) const { return std::tie( string_store, off, len ) == std::tie( that.string_store, that.off, that.len ); }

END_TL_NAMESPACE

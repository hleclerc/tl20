#include "../common_types.h"
#include <cstring>

BEG_TL_NAMESPACE

/**
    String with content stored in a pool
*/
class PString {
public:
    /**/     PString   ( auto &pool, const auto &str ) : _data( pool.allocate( str.size() ) ), _size( str.size() ) { std::memcpy( _data, str.data(), str.size() ); }
    /**/     PString   () : _data( nullptr ), _size( 0 ) {}

    operator Str       () const { return { _data, _size }; }

    PI       size      () const { return _size; }

    char*    _data;
    PI       _size;
};

END_TL_NAMESPACE

#include "../common_types.h"

BEG_TL_NAMESPACE

/**
*/
template<class T>
class PoolVec {
public:
    /**/     PoolVec   ( auto &pool, const auto &vec ) : _data( reinterpret_cast<T *>( pool.allocate( sizeof( T ) * vec.size() ) ) ), _size( vec.size() ) { for( PI i = 0; i < vec.size(); ++i ) new ( _data + i ) T( vec[ i ] ); }
    /**/     PoolVec   () : _data( nullptr ), _size( 0 ) {}
    
    const T& operator[]( PI i ) const { return _data[ i ]; }
    T&       operator[]( PI i ) { return _data[ i ]; }
    PI       size      () const { return _size; }

    T*       _data;
    PI       _size;
};

END_TL_NAMESPACE

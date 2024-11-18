#include "CompactReprReader.h"
#include "../ASSERT.h"
#include <utility>

BEG_TL_NAMESPACE

/*
s = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_-"
t = [ 0 for i in range( 256 ) ]
for i, c in enumerate( s ):
    t[ ord( c ) ] = i
print( 'const int CompactReprReader::number_table[] = {' + ','.join( map( str, t ) ) + '};' )
 */
const int CompactReprReader::number_table[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,63,0,0,0,1,2,3,4,5,6,7,8,9,0,0,0,0,0,0,0,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,0,0,0,0,62,0,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

CompactReprReader::CompactReprReader( StrView str ) : has_prefix( false ), offset( 0 ), str( str ) {
}


CompactReprReader::BI CompactReprReader::read_positive_int( const BI &max_value ) {
    read_prefix();
    
    BI res = prefix % max_value;
    prefix /= max_value;
    return res;
}

CompactReprReader::BI CompactReprReader::read_positive_int() {
    read_prefix();
    has_prefix = false;
    return prefix;
}

void CompactReprReader::read_prefix() {
    if ( has_prefix )
        return;
    has_prefix = true;

    prefix = 0;

    BI coeff = 1;
    for( ; offset < str.size() && number_table[ str[ offset ] ] > 32; ++offset, coeff *= 32 )
        prefix += coeff * ( number_table[ str[ offset ] ] - 32 );

    ASSERT( offset < str.size() );
    prefix += coeff * number_table[ str[ offset ] ];
    ++offset;
}

Str CompactReprReader::read_string() {
    PI size{ CompactReprReader::read_positive_int() };
    ASSERT( offset + size < str.size() );
    
    Str res = str.substr( offset, size );
    offset += size;
    return res;
}

END_TL_NAMESPACE

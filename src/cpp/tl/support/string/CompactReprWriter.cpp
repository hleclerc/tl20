#include "CompactReprWriter.h"

BEG_TL_NAMESPACE

const char CompactReprWriter::number_table[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_-"; // size = 64

CompactReprWriter::CompactReprWriter() : prefix_coeff( 1 ), prefix( 0 ) {
}
 
void CompactReprWriter::write_number( Str &res, BI value ) {
    for( ; value > 32; value /= 32 )
        res += number_table[ 32 + int( value % 32 ) ];
    res += number_table[ int( value ) ];
}

void CompactReprWriter::write_positive_int( const BI &value, const BI &max_value ) {
    prefix += prefix_coeff * value;
    prefix_coeff *= max_value;
}

void CompactReprWriter::write_positive_int( const BI &value ) {
    prefix += prefix_coeff * value;
    
    write_number( output, prefix );

    prefix_coeff = 1; 
    prefix = 0; 
}

void CompactReprWriter::write_string( StrView value ) {
    Str encoded;
    for( char c : value ) {
        if ( ( c >= '0' && c <= '9' ) || ( c >= 'a' && c <= 'z' ) || ( c >= 'A' && c <= 'Z' ) || c == '_' ) {
            encoded += c;
            continue;
        }
        encoded += '-';
        encoded += number_table[ c % 16 ];
        encoded += number_table[ c / 16 ];
    }

    write_positive_int( encoded.size() );
    output += encoded;
}

void CompactReprWriter::flush_prefix() {
    if ( prefix_coeff != 1 ) {
        write_number( output, prefix );
        prefix_coeff = 1;
        prefix = 0; 
    }
}

Str CompactReprWriter::str() const {
    Str res;
    if ( prefix_coeff != 1 )
        write_number( res, prefix );    
    return res + output;
}

END_TL_NAMESPACE

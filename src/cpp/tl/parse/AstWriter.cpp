#include "AstWriter.h"

BEG_TL_NAMESPACE

AstWriter::AstWriter() {
    str_len = 0;
}

AstWriterStr AstWriter::str( const Str &val ) {
    auto iter = str_map.find( val );
    if ( iter == str_map.end() )
        iter = str_map.insert( iter, { val, std::exchange( str_len, str_len + val.size() ) } );
    return { iter, 0, val.size() };
}

END_TL_NAMESPACE

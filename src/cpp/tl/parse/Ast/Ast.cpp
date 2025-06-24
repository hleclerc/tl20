#include "Ast.h"

BEG_TL_NAMESPACE

AString Ast::new_AString( StrView str ) {
    AString res;
    res._offset = strings.size();
    res._size = str.size();
    res._ast = this;

    strings.append( str );
    
    return res;
}

END_TL_NAMESPACE

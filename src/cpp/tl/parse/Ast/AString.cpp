#include "AString.h"
#include "Ast.h"

BEG_TL_NAMESPACE

AString::operator StrView() const {
    return { _ast->strings.data() + _offset, _size };
}

void AString::display( Displayer &dp ) const {
    dp << operator StrView();
}

END_TL_NAMESPACE



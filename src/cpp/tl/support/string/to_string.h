#pragma once

#include "../Displayer.h"

BEG_TL_NAMESPACE

template<class T>
std::string to_string( T &&val, const DisplayParameters &dp = {} ) {
    std::ostringstream ss;
    Displayer ds( &ss, dp );
    display( ds, val );

    return ss.str();
}

END_TL_NAMESPACE

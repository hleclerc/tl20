#pragma once

#include "../Displayer.h"

BEG_TL_NAMESPACE

template<class T>
std::string to_string( T &&val, const DisplayParameters &dp = {} ) {
    Displayer ds;
    ds << val;

    Str ss;
    ds.write_to( ss, dp );
    return ss;
}

END_TL_NAMESPACE

#pragma once

#include "../TODO.h"
#include "lu_solve.h"

BEG_TL_NAMESPACE

/// norm
auto determinant( auto &&M ) -> DECAYED_TYPE_OF( M[ 0 ][ 0 ] ) {
    if ( M.size() == 0 )
        return 1;

    if ( M.size() == 1 )
        return M[ 0 ][ 0 ];

    if ( M.size() == 2 ) {
        const auto &a = M[ 0 ][ 0 ], &b = M[ 0 ][ 1 ];
        const auto &c = M[ 1 ][ 0 ], &d = M[ 1 ][ 1 ];
        return a * d - b * c;
    }

    if ( M.size() == 3 ) {
        const auto &a = M[ 0 ][ 0 ], &b = M[ 0 ][ 1 ], &c = M[ 0 ][ 2 ];
        const auto &d = M[ 1 ][ 0 ], &e = M[ 1 ][ 1 ], &f = M[ 1 ][ 2 ];
        const auto &g = M[ 2 ][ 0 ], &h = M[ 2 ][ 1 ], &i = M[ 2 ][ 2 ];
        return a * e * i + b * f * g + c * d * h - g * e * c - h * f * a - i * d * b;
    }

    // auto res = M[ 0 ][ 0 ] * ;
    // for( std::size_t i = 1; i < M.size(); ++i )
    //     res += M[ i ] * M[ i ];
    TODO;
    return M[ 0 ][ 0 ];
}

END_TL_NAMESPACE

#pragma once

#include "../containers/Vec.h"
#include "../TODO.h"

BEG_TL_NAMESPACE

///
template<class A,int n>
Vec<A,n> lu_solve( const Vec<Vec<A,n>,n> &M, const Vec<A,n> &V ) {
    if ( n == 0 )
        return {};

    if ( n == 1 )
        return { V[ 0 ] / M[ 0 ][ 0 ] };

    if ( n == 2 ) {
        auto a = M[ 0 ][ 0 ], b = M[ 0 ][ 1 ];
        auto c = M[ 1 ][ 0 ], d = M[ 1 ][ 1 ];
        auto det = a * d - b * c;

        return {
            ( V[ 0 ] * d - V[ 1 ] * b ) / det,
            ( a * V[ 1 ] - c * V[ 0 ] ) / det
        };
    }

    if ( n == 3 ) {
        const auto &a = M[ 0 ][ 0 ], &b = M[ 0 ][ 1 ], &c = M[ 0 ][ 2 ];
        const auto &d = M[ 1 ][ 0 ], &e = M[ 1 ][ 1 ], &f = M[ 1 ][ 2 ];
        const auto &g = M[ 2 ][ 0 ], &h = M[ 2 ][ 1 ], &i = M[ 2 ][ 2 ];

        const auto det = []( auto a, auto b, auto c, auto d, auto e, auto f, auto g, auto h, auto i ) {
            return a * e * i + b * f * g + c * d * h - g * e * c - h * f * a - i * d * b;
        };

        auto bdet = det( a, b, c, d, e, f, g, h, i );

        return {
            det( V[ 0 ], b, c, V[ 1 ], e, f, V[ 2 ], h, i ) / bdet,
            det( a, V[ 0 ], c, d, V[ 1 ], f, g, V[ 2 ], i ) / bdet,
            det( a, b, V[ 0 ], d, e, V[ 1 ], g, h, V[ 2 ] ) / bdet,
        };
    }

    using std::abs;
    // auto res = a[ 0 ][ 0 ] * ;
    // for( std::size_t i = 1; i < a.size(); ++i )
    //     res += a[ i ] * a[ i ];
    TODO;
    return {}; // res;
}

END_TL_NAMESPACE

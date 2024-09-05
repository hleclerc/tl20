#include <tl/support/containers/Vec.h>
#include "catch_main.h"

TEST_CASE( "Vec", "" ) {
    Vec<int> v;
    v << 10 << 20;
    P( v );
    P( v * v );
}

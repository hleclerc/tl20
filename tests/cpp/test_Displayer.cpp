#include "tl/support/containers/Vec.h"
#include "tl/support/display.h"
#include "catch_main.h"
#include <iostream>

struct Pouet {
    void for_each_attribute( auto &&func ) const { func( "a", a ); func( "b", b ); }
    int a = 10;
    Vec<int> b = { 1, 2 };
};

TEST_CASE( "Displayer", "" ) {
    // Displayer ds;

    // ds.append_pointer( true, "yo", [&]() { ds << 19; } );

    // Str res;
    // ds.write_to( res, { .ensure_endline = true } );
    // std::cout << res;
    
    display( std::cout, Pouet() );
    std::cout << std::endl;
}

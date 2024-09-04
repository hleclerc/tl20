#include "catch_main.h"
#include <iostream>

TEST_CASE( "Displayer", "" ) {
    Displayer ds;

    ds.start_list();
    ds << 17;
    ds << 18;
    ds.end_list();

    Str res;
    ds.write_to( res, { .ensure_endline = true } );
    std::cout << res;
}

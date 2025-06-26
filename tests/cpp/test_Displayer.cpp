#include "tl/support/containers/Vec.h"
#include "tl/support/display.h"
#include "catch_main.h"
#include <iostream>

struct Pouet {
    virtual void display( Displayer &dp ) const = 0;
};

struct Yo : Pouet {
    void for_each_attribute( auto &&func ) const { func( "a", a ); func( "b", b ); func( "next", next ); }
    virtual void display( Displayer &dp ) const override { display_object( dp, *this ); }

    int a = 10;
    Vec<int> b = { 1, 2 };
    Pouet *next = nullptr;
};

TEST_CASE( "Displayer", "" ) {
    Yo a;
    Yo b;
    a.next = &b;
    P( a );
}

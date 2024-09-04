#pragma once

#include "display/DisplayParameters.h"
#include "memory/BumpPointerPool.h"
#include <functional>

BEG_TL_NAMESPACE
class DisplayItem;

/**
 * @brief 
 * 
 */
class Displayer {
public:
    struct          Number       { Str numerator, denominator, shift, base_shift; }; ///< values are represented in base 10.
   
    /**/            Displayer    ();
    /**/           ~Displayer    ();

    void            set_next_name( const Str &name ); ///< set the name of the next item to be appended
    TT Displayer&   operator<<   ( const T &value ) { display( *this, value ); return *this; }
    virtual void    write_to     ( Str &out, const DisplayParameters &dp ) const;

    // helpers
    void            append_number( const Number &number );
    void            append_string( const Str &str );
    void            append_list  ( const std::function<void()> &cb );

    void            start_list   ();
    void            end_list     ();

    Str             next_name;
    DisplayItem*    last;
    BumpPointerPool pool;
};

void display( Displayer &ds, SI32 value );

END_TL_NAMESPACE

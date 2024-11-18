#pragma once

#include <boost/multiprecision/cpp_int.hpp>
#include "../common_types.h"

BEG_TL_NAMESPACE

/**
 * @brief ease "compact representation" writing
 * 
 * A compact representation begins with a encoded number with all the bounded values + the first unbounded one (or 0 if no unbounded value)
 *   it is followed by 
 */
class CompactReprWriter {
public:
    using              BI                = boost::multiprecision::cpp_int; ///< num and den types
 
    /* */              CompactReprWriter ();
             
    void               write_positive_int( const BI &value, const BI &max_value );
    void               write_positive_int( const BI &value );
    void               write_string      ( StrView value );
             
    Str                str               () const;
 
    CompactReprWriter& operator<<        ( const auto &v ) { v.write_to( *this ); return *this; }
    CompactReprWriter& operator<<        ( const Str &v ) { write_string( v ); return *this; }
 
private: 
    static const char  number_table     []; ///<
     
    static void        write_number     ( Str &res, BI value );
    void               flush_prefix     ();
             
    BI                 prefix_coeff;
    BI                 prefix;
    Str                output;
};

END_TL_NAMESPACE

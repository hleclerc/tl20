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
class CompactReprReader {
public:
    using             BI               = boost::multiprecision::cpp_int; ///< num and den types

    /* */             CompactReprReader( StrView str );
            
    BI                read_positive_int( const BI &max_value );
    BI                read_positive_int();
    Str               read_string      ();
    operator          bool             () const { return offset == str.size(); }
    
private:
    static const int  number_table     []; ///<

    void              read_prefix      (); ///<

    bool              has_prefix;      ///< 
    BI                prefix;          ///< 
    PI                offset;          ///< in str
    Str               str;             ///<
};

END_TL_NAMESPACE

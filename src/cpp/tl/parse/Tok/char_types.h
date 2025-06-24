#include "../../support/common_types.h"

BEG_TL_NAMESPACE
namespace Tok {

inline bool letter ( char c ) { return ( c >= 'a' && c <= 'z' ) || ( c >= 'A' && c <= 'Z' ) || c == '_'; }
inline bool number ( char c ) { return c >= '0' && c <= '9'; }
inline bool space  ( char c ) { return c == ' ' || c == '\t' || c == '\r'; }
 
inline bool oper   ( char c ) {
     return c == '+' || c == '~' || c == '-' || c == '|' || c == '\\' || c == '^' || 
            c == '@' || c == '=' || c == '+' || c == '"' || c == '$'  || c == '%' || 
            c == '*' || c == '!' || c == ':' || c == '/' || c == '.'  || c == '?' || 
            c == '<' || c == '>' || c == '"' || c == '`' || c == '\'' ;
}

inline bool alphan ( char c ) { return letter( c ) || number( c ); }

// cnt ====================================================================================================
inline bool is_cnt_for_variable( char c, const Str &prev_char_value ) { return alphan( c ); }
inline bool is_cnt_for_new_line( char c, const Str &prev_char_value ) { return space ( c ); }
inline bool is_cnt_for_operator( char c, const Str &prev_char_value ) { return oper  ( c ); }
inline bool is_cnt_for_number  ( char c, const Str &prev_char_value ) {
    if ( c == '+' || c == '-' )
        return prev_char_value.ends_with( 'e' ) || prev_char_value.ends_with( 'E' );
    return alphan( c );
}

} // namespace Tok
END_TL_NAMESPACE

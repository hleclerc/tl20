#include "tl/support/tl_namespace.h"

BEG_TL_NAMESPACE
namespace Tok {

inline bool letter( int c ) { return ( c >= 'a' && c <= 'z' ) || ( c >= 'A' && c <= 'Z' ) || c == '_'; }
inline bool number( int c ) { return c >= '0' && c <= '9'; }
inline bool space ( int c ) { return c == ' ' || c == '\t' || c == '\r'; }

inline bool oper  ( int c ) {
     return c == '+' || c == '~' || c == '-' || c == '|' || c == '\\' || c == '^' || 
            c == '@' || c == '=' || c == '+' || c == '"' || c == '$'  || c == '%' || 
            c == '*' || c == '!' || c == ':' || c == '/' || c == '.'  || c == '?' || 
            c == '<' || c == '>' || c == '"' || c == '`' || c == '\'' ;
}

inline bool alphan( int c ) { return letter( c ) || number( c ); }

inline bool is_cnt_for_variable( int c ) { return alphan( c ); }
inline bool is_cnt_for_new_line( int c ) { return space ( c ); }
inline bool is_cnt_for_operator( int c ) { return oper  ( c ); }

inline bool is_cnt_for_number( int c, int prev_char_value ) {
    if ( c == '+' || c == '-' )
        return prev_char_value == 'e' || prev_char_value == 'E';
    return alphan( c );
}

} // namespace Tok
END_TL_NAMESPACE

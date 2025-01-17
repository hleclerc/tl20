#include "../support/TODO.h"
#include "../support/P.h"
#include <utility>
#include "TlParser.h"

BEG_TL_NAMESPACE

inline bool letter( int c ) { return ( c >= 'a' && c <= 'z' ) || ( c >= 'A' && c <= 'Z' ) || c == '_' || c == '$'; }
inline bool number( int c ) { return c >= '0' && c <= '9'; }
inline bool space ( int c ) { return c == ' ' || c == '\t' || c == '\r'; }
inline bool oper  ( int c ) { return c == '+' || c == '~' || c == '-' || c == '|' || c == '\\' || c == '^' || c == '@' || c == '=' || c == '+' || c == '"' || c == '$' || c == '%'  || c == '*' || c == '!' || c == ':' || c == '/' || c == '.' || c == '?' || c == '<' || c == '>' || c == '"' || c == '`' || c == '\''; }

inline bool alphan( int c ) { return letter( c ) || number( c ); }

inline bool is_cnt_for_variable( int c ) { return alphan( c ); }
inline bool is_cnt_for_new_line( int c ) { return space ( c ); }
inline bool is_cnt_for_operator( int c ) { return oper  ( c ); }

inline bool is_cnt_for_number( int c, int prev_char_value ) {
    if ( c == '+' || c == '-' )
        return prev_char_value == 'e' || prev_char_value == 'E';
    return alphan( c );
}


TlParser::TlParser() {
    _init();
}

void TlParser::_init() {
    curr_tok_content.clear();
    restart_jump = nullptr;
}

void TlParser::_parse( int c, const char *nxt, const char *beg, const char *end, AstWriterStr src_url ) {
    // restart (jump to the right place)
    if ( ! restart_jump )
        restart_jump = &&cnt_new_line;
    goto *restart_jump;

    // when nothing has started (not in a variable, number, ...)
    char_switch:
        if ( letter( c ) ) goto beg_variable;
        if ( number( c ) ) goto beg_number;
        if ( space ( c ) ) goto beg_space;
        if ( oper  ( c ) ) goto beg_operator;
        if ( c == '\n'   ) goto beg_new_line;
        if ( c == '#'    ) goto beg_comment;
        // if ( c == '('    ) { on_opening_paren( Token::Type::ParenthesisCall, ')' ); goto inc_switch; }
        // if ( c == ')'    ) { on_closing_paren( ')' ); goto inc_switch; }
        // if ( c == ';'    ) { on_semicolon(); goto inc_switch; }
        // if ( c == ','    ) { on_comma(); goto inc_switch; }
        if ( c == eof    ) return;
        // on_error( va_string( "Unknown char type '{0}'", char( c ) ) );
        TODO;
        return;
    inc_switch:
        if ( nxt == end ) {
            restart_jump = &&char_switch;
            return;
        }
        c = *( nxt++ );

    // comment
    beg_comment:
    cnt_comment:
        if ( nxt == end )
            goto int_comment;
        c = *( nxt++ );
        if ( c == '\n' )
            goto beg_new_line;
        goto cnt_comment;
    int_comment:
        restart_jump = &&cnt_comment;
        return;

    // spacing
    beg_space:
        _on_space();
    cnt_space:
        if ( nxt == end )
            goto int_space;
        c = *( nxt++ );
        if ( space( c ) )
            goto cnt_space;
        goto char_switch;
    int_space:
        restart_jump = &&cnt_space;
        return;

    // number
    beg_number:
        curr_tok_src_off = nxt - beg - 1;
        curr_tok_src_url = src_url;
        curr_tok_content.clear();
        prev_char_value = c;
    psh_number:
        curr_tok_content += c;
        if ( nxt == end )
            goto int_number;
        prev_char_value = c;
        c = *( nxt++ );
    cnt_number:
        if ( is_cnt_for_number( c, prev_char_value ) )
            goto psh_number;
        _on_number();
        goto char_switch;
    int_number:
        restart_jump = &&cnt_number;
        return;

    #define PARSE_TYPE( TYPE ) \
        beg_##TYPE: \
            curr_tok_src_off = nxt - beg - 1; \
            curr_tok_src_url = src_url; \
            curr_tok_content.clear(); \
        psh_##TYPE: \
            curr_tok_content += c; \
            if ( nxt == end ) \
                goto int_##TYPE; \
            c = *( nxt++ ); \
        cnt_##TYPE: \
            if ( is_cnt_for_##TYPE( c ) ) \
                goto psh_##TYPE; \
            _on_##TYPE(); \
            goto char_switch; \
        int_##TYPE: \
            restart_jump = &&cnt_##TYPE; \
            return;

    PARSE_TYPE( new_line );
    PARSE_TYPE( variable );
    PARSE_TYPE( operator );

    #undef PARSE_TYPE
}

void TlParser::parse( StrView content, PI src_off, AstWriterStr src_url ) {
    if ( content.empty() )
        return;

    const char *cur = content.begin();
    const char *end = content.end();
    _parse( *cur, cur + 1, cur - src_off, end, src_url );
}

void TlParser::dump( AstWriter &writer ) {
    _parse( eof, nullptr, nullptr, nullptr, {} );
    _init();
}

void TlParser::_on_new_line() {

}

void TlParser::_on_variable() {
    P( "v", curr_tok_content );
}

void TlParser::_on_operator() {
    P( "o", curr_tok_content );
}

void TlParser::_on_number() {
    P( "n", curr_tok_content );
}

void TlParser::_on_space() {

}

END_TL_NAMESPACE

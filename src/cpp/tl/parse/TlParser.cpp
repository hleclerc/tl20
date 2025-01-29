#include "../support/string/va_string.h"
#include "../support/ASSERT.h"
#include "../support/ERROR.h"
#include "../support/TODO.h"
#include "../support/P.h"
#include "tl/parse/OperatorTrie.h"
#include "tl/parse/TlToken.h"
#include <limits>
#include "TlParser.h"

BEG_TL_NAMESPACE

// char helpers -------------------------------------------------------------------------------------------------------------------
inline bool letter( int c ) { return ( c >= 'a' && c <= 'z' ) || ( c >= 'A' && c <= 'Z' ) || c == '_'; }
inline bool number( int c ) { return c >= '0' && c <= '9'; }
inline bool space ( int c ) { return c == ' ' || c == '\t' || c == '\r'; }
inline bool oper  ( int c ) { return c == '+' || c == '~' || c == '-' || c == '|' || c == '\\' || c == '^' || 
                                     c == '@' || c == '=' || c == '+' || c == '"' || c == '$'  || c == '%' || 
                                     c == '*' || c == '!' || c == ':' || c == '/' || c == '.'  || c == '?' || 
                                     c == '<' || c == '>' || c == '"' || c == '`' || c == '\'' ; }

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
    operator_trie = OperatorTrie::default_tl_operator_trie();
    _init();
}

void TlParser::display( Displayer &ds ) const {
    ds << *token_stack.front().token;
}

void TlParser::_init() {
    curr_tok_src_refs.clear();
    curr_tok_content.clear();
    restart_jump = nullptr;

    just_seen_a_semicolon = false;
    just_seen_a_new_line = false;
    just_seen_a_comma = false;
    just_seen_a_space = false;
    prev_line_beg.clear();

    token_stack.clear();
    pool.clear();
}

void TlParser::_parse( int c, const char *nxt, const char *beg, const char *end, AstWriterStr src_url ) {
    // restart (i.e. jump to the right place)
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
        if ( c == '('    ) { _on_opening_paren( TlToken::Type::ParenthesisCall, ')' ); goto inc_switch; }
        if ( c == '['    ) { _on_opening_paren( TlToken::Type::BracketCall    , ']' ); goto inc_switch; }
        if ( c == '{'    ) { _on_opening_paren( TlToken::Type::BraceCall      , '}' ); goto inc_switch; }
        if ( c == ')'    ) { _on_closing_paren( ')' ); goto inc_switch; }
        if ( c == ']'    ) { _on_closing_paren( ']' ); goto inc_switch; }
        if ( c == '}'    ) { _on_closing_paren( '}' ); goto inc_switch; }
        if ( c == ';'    ) { _on_semicolon(); goto inc_switch; }
        if ( c == ','    ) { _on_comma(); goto inc_switch; }
        if ( c == eof    ) return;

        curr_tok_src_refs = { SrcRef{ src_url, PI( nxt - beg - 1 ), PI( nxt - beg ) } };
        _error( va_string( "Unknown char type '{0}'", char( c ) ) );
        return;

    inc_switch:
        if ( nxt == end ) {
            restart_jump = &&char_switch;
            return;
        }
        c = *( nxt++ );
        goto char_switch;

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
    cnt_space:
        if ( nxt == end )
            goto int_space;
        c = *( nxt++ );
        if ( space( c ) )
            goto cnt_space;
        _on_space();
        goto char_switch;
    int_space:
        restart_jump = &&cnt_space;
        return;

    // number
    beg_number:
        curr_tok_src_refs = { SrcRef{ src_url, PI( nxt - beg - 1 ) } };
        curr_tok_content.clear();
    psh_number:
        curr_tok_content += c;
        if ( nxt == end )
            goto int_number;
        prev_char_value = c;
        c = *( nxt++ );
    cnt_number:
        if ( is_cnt_for_number( c, prev_char_value ) )
            goto psh_number;
        curr_tok_src_refs.back().end = nxt - beg - 1;
        _on_number();
        goto char_switch;
    int_number:
        curr_tok_src_refs.back().end = nxt - beg - 1;
        restart_jump = &&res_number;
        prev_char_value = c;
        return;
    res_number:
        if ( curr_tok_src_refs.back().url != src_url || curr_tok_src_refs.back().end != beg - nxt - 1 )
            curr_tok_src_refs.push_back_br( src_url, PI( nxt - beg - 1 ) );
        goto cnt_number;

    #define PARSE_TYPE( TYPE ) \
        beg_##TYPE: \
            curr_tok_src_refs = { SrcRef{ src_url, PI( nxt - beg - 1 ) } }; \
            curr_tok_content.clear(); \
        psh_##TYPE: \
            curr_tok_content += c; \
            if ( nxt == end ) \
                goto int_##TYPE; \
            c = *( nxt++ ); \
        cnt_##TYPE: \
            if ( is_cnt_for_##TYPE( c ) ) \
                goto psh_##TYPE; \
            curr_tok_src_refs.back().end = nxt - beg - 1; \
            _on_##TYPE(); \
            goto char_switch; \
        int_##TYPE: \
            curr_tok_src_refs.back().end = nxt - beg - 1; \
            restart_jump = &&res_##TYPE; \
            return; \
        res_##TYPE: \
            if ( curr_tok_src_refs.back().url != src_url || curr_tok_src_refs.back().end != beg - nxt - 1 ) \
                curr_tok_src_refs.push_back_br( src_url, PI( nxt - beg - 1 ) ); \
            goto cnt_##TYPE;

    PARSE_TYPE( new_line );
    PARSE_TYPE( variable );
    PARSE_TYPE( operator );

    #undef PARSE_TYPE
}

void TlParser::_on_opening_paren( TlToken::Type call_type, char expected_closing ) {

}

void TlParser::_on_closing_paren( char c ) {

}

void TlParser::_on_semicolon() {

}

void TlParser::_on_comma() {
    if ( just_seen_a_semicolon )
        return _error( "a comma just after a semicolon does not have a defined behavior." );
    if ( just_seen_a_new_line )
        return _error( "a comma at the beginning of a new line does not have a defined behavior." );
    if ( just_seen_a_comma )
        return _error( "a comma just after another comma does not have a defined behavior." );

    // gp up
    if ( token_stack.back().on_a_new_line || token_stack.back().closing_char ) {
        _error( "impossible to go higher" );
        return;
    }
    token_stack.pop_back();

    // ... to something that accepts children
    while ( token_stack.back().max_nb_children == 0 )
        token_stack.pop_back();
}

void TlParser::_update_stack_after_nl() {
    while ( token_stack.size() ) {
        const StackItem &si = token_stack.back();
        if ( si.closing_char ) {
            break;
        }
        if ( si.on_a_new_line && si.newline_size < prev_line_beg.size() ) {
            break;
        }
        token_stack.pop_back();
    }
}

void TlParser::_add_child_to( TlToken *parent, TlToken *child ) {
    if ( parent->first_child )
        parent->last_child->next = child;
    else
        parent->first_child = child;
    child->prev = parent->last_child;
    parent->last_child = child;
    child->parent = parent;
}

void TlParser::_push_token( TlToken::Type type, StrView content, int right_prio ) {
    // ASSERT( curr_tok_src_url == src_url ); // for now tokens must be in the same src

    // update the stack
    if ( just_seen_a_new_line )
        _update_stack_after_nl();

    //
    TlToken *token = pool.create<TlToken>();
    token->src_refs = { pool, curr_tok_src_refs };
    token->content = content;
    token->type = type;

    //
    _add_child_to( token_stack.back().token, token );

    //
    token_stack << StackItem{
        .token = token,
        .closing_char = 0,
        .on_a_new_line = just_seen_a_new_line,
        .newline_size = int( prev_line_beg.size() ),
        .max_nb_children = std::numeric_limits<int>::max(),
        .prio = right_prio
    };

    //
    just_seen_a_semicolon = false;
    just_seen_a_new_line = false;
    just_seen_a_comma = false;
    just_seen_a_space = false;
}

void TlParser::parse( StrView content, PI src_off, AstWriterStr src_url ) {
    // create the root token if not already done
    if ( token_stack.empty() ) {
        curr_tok_src_refs = { SrcRef{ src_url, src_off, src_off } };

        TlToken *token = pool.create<TlToken>();
        token->src_refs = { pool, curr_tok_src_refs };
        token->type = TlToken::Type::Root;
    
        token_stack << StackItem{
            .token = token,
            .closing_char = -1,
            .on_a_new_line = true,
            .newline_size = -1,
            .max_nb_children = std::numeric_limits<int>::max(),
            .prio = -1
        };
    }

    // empty string -> nothing to do
    if ( content.empty() )
        return;

    //
    const char *cur = content.begin();
    const char *end = content.end();
    _parse( *cur, cur + 1, cur - src_off, end, src_url );
    _parse( eof, end, end - src_off, end, src_url );
}

void TlParser::dump( AstWriter &writer ) {
    _parse( eof, nullptr, nullptr, nullptr, {} );
    _init();
}

void TlParser::_error( Str msg ) {
    ERROR( msg );
}

void TlParser::_on_new_line() {
    PI oc = curr_tok_content.starts_with( '\n' );
    PI op = prev_line_beg.starts_with( '\n' );
    for( PI i = 0; i < std::min( curr_tok_content.size() - oc, prev_line_beg.size() - op ); ++i ) {
        if ( curr_tok_content[ i + oc ] != prev_line_beg[ i + op ] ) {
            _error( "incoherent spacing between the beginning of this line and the previous one" );
            break;
        }
    }

    prev_line_beg = curr_tok_content;
    just_seen_a_new_line = true;
}

void TlParser::_on_variable() {
    _push_token( TlToken::Type::Variable, curr_tok_content, 0 );
}

void TlParser::_take_left( TlToken::Type type, StrView content, int right_prio, int left_prio ) {
    P( content );
    TODO;
}

void TlParser::_on_operator() {
    StrView str = curr_tok_content;
    while ( OperatorTrie::OperatorData *od = operator_trie->symbol_op( str ) ) {
        if ( od->take_left )
            _take_left( TlToken::Type::Variable, od->name, od->priority, od->priority - od->l2r );
        else 
            _push_token( TlToken::Type::Variable, od->name, od->priority );

        curr_tok_src_refs.back().beg += od->str.size();
        str.remove_prefix( od->str.size() );
    }

    if ( ! str.empty() )
        _error( va_string( "there's no registered operator in '$0' or subparts of it.", str ) );
}

void TlParser::_on_number() {
    _push_token( TlToken::Type::Number, curr_tok_content, 0 );
}

void TlParser::_on_space() {
    just_seen_a_space = true;
}

END_TL_NAMESPACE

#include "../support/string/va_string.h"
#include "../support/ASSERT.h"
#include "Tok/char_types.h"
#include "TokFromTxt.h"
#include <limits>

BEG_TL_NAMESPACE
using namespace Tok;

TokFromTxt::TokFromTxt( Log &log ) : log( log ) {
    _init();
}

Tok::Node *TokFromTxt::root() {
    return token_stack.front().token;
}

void TokFromTxt::_init() {
    operator_trie = base_operator_trie();

    // pending_...
    prev_token_is_touching = false;
    pending_new_line = true;
    pending_comma = false;

    // other attributes
    curr_tok_src_refs.clear();
    curr_tok_content.clear();
    prev_char_value = -1;
    prev_line_beg.clear();
    restart_jump = nullptr;
    pool.clear();

    // token_stack
    token_stack = { StackItem{
        .token = _new_node( Node::Type::Root ),
        .closing_char = -1,
        .on_a_new_line = true,
        .newline_size = -1,
        .max_nb_children = std::numeric_limits<int>::max(),
        .prio = std::numeric_limits<int>::max(),
    } };
}

void TokFromTxt::_parse( const char *cur, const char *beg, const char *end ) {
    // if no char -> nothing to do
    if ( cur == end )
        return;

    // else, read the char
    char c = *( cur++ );

    // jump to the right place
    if ( ! restart_jump )
        restart_jump = &&cnt_new_line;
    goto *restart_jump;

    // after an eof =======================================================================================================
    continuation_error: restart_jump = &&beg_eof; return _error( "a continuation byte was not expected here" );
    after_eof: restart_jump = &&beg_eof; return;

    // beg generated code
    // end generated code

    // when nothing has started (not in a variable, number, ...)
    char_switch:
        if ( letter( c ) ) { curr_tok_content = c; goto beg_variable; }
        if ( number( c ) ) { curr_tok_content = c; goto beg_number; }
        if ( space ( c ) ) { goto beg_space; }
        if ( oper  ( c ) ) { curr_tok_content = c; goto beg_operator; }
        if ( c == '\n'   ) { goto beg_new_line; }
        if ( c == '#'    ) { goto beg_comment; }
        if ( c == '('    ) { _on_opening_paren( Node::Type::ParenthesisCall, "operator ()", ')' ); goto inc_switch; }
        if ( c == '['    ) { _on_opening_paren( Node::Type::BracketCall    , "operator []", ']' ); goto inc_switch; }
        if ( c == '{'    ) { _on_opening_paren( Node::Type::BraceCall      , "operator {}", '}' ); goto inc_switch; }
        if ( c == ')'    ) { _on_closing_paren( ')' ); goto inc_switch; }
        if ( c == ']'    ) { _on_closing_paren( ']' ); goto inc_switch; }
        if ( c == '}'    ) { _on_closing_paren( '}' ); goto inc_switch; }
        if ( c == ';'    ) { _on_semicolon(); goto inc_switch; }
        if ( c == ','    ) { _on_comma(); goto inc_switch; }

        if ( c == 0xFF   ) { restart_jump = &&restart_loop; return; }

        if ( c >= 0x80 ) {
            if ( c <= 0xBF )   // Continuation bytes			10xxxxxx	0x80 - 0xBF
                return _error( "invalid utf8 encoding (continuation byte at the wrong place)" );
            if ( c <= 0xDF ) { // 2 byte sequence introducer	110xxxxx	0xC0 - 0xDF
                curr_codepoint = c & ( 0xFF >> 3 );
                goto inc_switch_codepoint_1;
            }
            if ( c <= 0xEF ) { // 3 byte sequence introducer	1110xxxx	0xE0 - 0xEF

            }
            if ( c <= 0xF7 ) { // 4 byte sequence introducer	11110xxx	0xF0 - 0xF7
            }
            if ( c <= 0xFB ) { // 5 byte sequence introducer	111110xx	0xF8 - 0xFB
            }
            if ( c <= 0xFD ) { // 6 byte sequence introducer	1111110x	0xFC - 0xFD
            }

            _error( "invalid utf8 encoding (Unassigned 11111110 byte)" );
            goto inc_switch;
        }

        return _error( va_string( "Unknown char type '{0}'", char( c ) ) );

    inc_switch:
        if ( cur == end ) {
            restart_jump = &&char_switch;
            return;
        }
        c = *( cur++ );
        goto char_switch;

    // ==================================================================================================================
    inc_switch_codepoint__:
        #define INC_SWITCH_CODEPOINT( N ) \
            inc_switch_codepoint_##N: \
                if ( cur == end ) { \
                    restart_jump = &&cnt_switch_codepoint_##N; \
                    return; \
                } \
                c = *( cur++ ); \
            cnt_switch_codepoint_##N: \
                if ( ( c & 0b11000000 ) != 0b10000000 ) \
                    return _error( "Expecting an utf8 continuation at this place" ); \
                curr_codepoint = ( curr_codepoint << 6 ) | ( c & 0b00111111 );
        INC_SWITCH_CODEPOINT( 6 )
        INC_SWITCH_CODEPOINT( 5 )
        INC_SWITCH_CODEPOINT( 4 )
        INC_SWITCH_CODEPOINT( 3 )
        INC_SWITCH_CODEPOINT( 2 )
        #undef INC_SWITCH_CODEPOINT

        goto codepoint_switch;

    // ==================================================================================================================
    codepoint_switch:
        TODO;


    // comment ============================================================================================================
    beg_comment:
    cnt_comment:
        if ( cur == end )
            goto int_comment;
        c = *( cur++ );
        if ( c == '\n' )
            goto beg_new_line;
        goto cnt_comment;
    int_comment:
        restart_jump = &&cnt_comment;
        return;

    // spacing ============================================================================================================
    beg_space:
    cnt_space:
        if ( cur == end )
            goto int_space;
        c = *( cur++ );
        if ( space( c ) )
            goto cnt_space;
        _on_space();
        goto char_switch;
    int_space:
        restart_jump = &&cnt_space;
        return;

    // ====================================================================================================================
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
    PARSE_TYPE( number   );

    #undef PARSE_TYPE
}

void TokFromTxt::_on_opening_paren( Node::Type call_type, const char *func_name, char expected_closing ) {
    if ( prev_token_is_touching ) { // `some_code( a, b )` -> `a` and `b` will be the children of `some_code`
        Node *token_func = _new_node( Node::Type::ParenthesisCall );
        bool took = _take_node( token_func, {
            .max_nb_children = std::numeric_limits<int>::max(),
            .min_nb_children = 0,
            .right_prio = operator_trie->prio_call,
            .left_prio = operator_trie->prio_call
        } );
        ASSERT( took );
    } else { // `some_code ( a, b ), c` -> `( a, b )` and `c` will be the children of `some_code`
        Node *token_name = _new_node( Node::Type::Variable, func_name );
        Node *token_func = _new_node( Node::Type::ParenthesisCall );
        token_func->add_child( token_name );
        _append_token( token_func, {
            .max_nb_children = std::numeric_limits<int>::max(),
            .min_nb_children = 0,
            .right_prio = operator_trie->prio_call,
        } );
    }

    token_stack.back().closing_char = expected_closing;

    prev_token_is_touching = true;
    pending_new_line = false;
    pending_comma = false;
}

void TokFromTxt::_on_closing_paren( char c ) {
    while ( token_stack.back().closing_char == 0 )
        _pop_stack_item();

    if ( token_stack.back().closing_char != c )
        return _error( va_string( "'$0' has no correspondance", c ) );

    // 
    StackItem &si = token_stack.back();
    si.max_nb_children = 0;
    si.closing_char = 0;

    //
    prev_token_is_touching = true;
    pending_new_line = false;
    pending_comma = false;
}

void TokFromTxt::_on_semicolon() {
    // gp up
    while ( token_stack.back().closing_char == 0 && token_stack.back().on_a_new_line == false )
        _pop_stack_item();

    // newline => go to parent
    if ( token_stack.back().closing_char == 0 )
        _pop_stack_item();

    prev_token_is_touching = false;
    pending_new_line = false;
    pending_comma = false;
}

void TokFromTxt::_on_comma() {
    prev_token_is_touching = false;

    if ( pending_new_line ) {
        _update_stack_after_newline();
        pending_new_line = false;
    }

    if ( pending_comma ) 
        return _error( "a comma just after another comma does not have a defined behavior." );
    pending_comma = true;
}

void TokFromTxt::_update_stack_after_newline() {
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

void TokFromTxt::_update_stack_after_comma() {
    const StackItem &si = token_stack.back();
    if ( si.closing_char == 0 )
        _pop_stack_item();
}
    
Node *TokFromTxt::_new_node( Node::Type type, StrView content ) {
    // create the new token
    Node *token = pool.create<Node>();
    token->src_refs = { pool, curr_tok_src_refs };
    token->content = { pool, content };
    token->type = type;
    return token;
}

void TokFromTxt::_pop_stack_item() {
    if ( token_stack.size() == 1 )
        return _error( "parse stack is exhausted" );

    StackItem &si = token_stack.back();
    if ( si.min_nb_children > 0 ) {
        if ( si.min_nb_children == si.max_nb_children ) {
            if ( si.min_nb_children == 1 )
                _error( va_string( "token was expecting an additional child." ), { token_stack.back().token } );
            else
                _error( va_string( "token was expecting $0 additional children.", si.min_nb_children ), { token_stack.back().token } );
        } else {
            if ( si.min_nb_children == 1 )
                _error( va_string( "token was expecting at least one additional child." ), { token_stack.back().token } );
            else
                _error( va_string( "token was expecting at least $0 additional children.", si.min_nb_children ), { token_stack.back().token } );
        }
    }

    token_stack.pop_back();
}

void TokFromTxt::parse_txt( StrView content, StrRef src_url, PI src_off ) {
    // empty string -> nothing to do
    if ( content.empty() )
        return;

    //
    curr_tok_src_refs << SrcRef{ .url = src_url, .beg = src_off, .end = -1 };
    _parse( content.begin(), content.begin() - src_off, content.end() );
    curr_tok_src_refs.back().end = src_off + content.size();
}

void TokFromTxt::parse_eof() {
    const char eof = 0xFF;
    _parse( &eof, &eof, &eof + 1 );
}

void TokFromTxt::_error( Str msg, Vec<Node *> tok ) {
    log.msg( msg, {}, Log::Type::ERROR );
}

void TokFromTxt::_on_new_line() {
    PI oc = curr_tok_content.starts_with( '\n' );
    PI op = prev_line_beg.starts_with( '\n' );
    for( PI i = 0; i < std::min( curr_tok_content.size() - oc, prev_line_beg.size() - op ); ++i ) {
        if ( curr_tok_content[ i + oc ] != prev_line_beg[ i + op ] ) {
            _error( "incoherent spacing between the beginning of this line and the previous one" );
            break;
        }
    }

    prev_line_beg = curr_tok_content;

    prev_token_is_touching = false;
    pending_new_line = true;
    pending_comma = false;
}

void TokFromTxt::_on_variable() {
    Node *tok = _new_node( Node::Type::Variable, curr_tok_content );
    _append_token( tok, { .max_nb_children = 0 } );

    prev_token_is_touching = true;
    pending_new_line = false;
    pending_comma = false;
}

void TokFromTxt::_on_operator() {
    auto use_operator = [&]( OperatorTrie::OperatorData *od, Node *tok_call ) -> void {
        if ( od->take_left ) {
            // try to take a token
            bool took = _take_node( tok_call, {
                .min_nb_children = od->min_rch,
                .max_nb_children = od->max_rch,
                .right_prio = od->priority,
                .left_prio = od->priority - od->l2r,
            } );
            if ( took )
                return;

            // error if it was mandatory
            if ( od->take_left >= 2 )
                return _error( "nothing to take on the left" );
        }

        _append_token( tok_call, {
            .min_nb_children = od->min_rch,
            .max_nb_children = od->max_rch,
            .right_prio = od->priority,
        } );
    };

    StrView str = curr_tok_content;
    while ( OperatorTrie::OperatorData *od = operator_trie->symbol_op( str ) ) {
        Node *tok_func = _new_node( Node::Type::Variable, od->name );
        Node *tok_call = _new_node( Node::Type::ParenthesisCall );
        tok_call->add_child( tok_func );

        use_operator( od, tok_call );

        prev_token_is_touching = true;
        pending_new_line = false;
        pending_comma = false;

        curr_tok_src_refs.back().beg += od->str.size();
        str.remove_prefix( od->str.size() );
    }

    if ( ! str.empty() )
        _error( va_string( "there's no registered operator in '$0' or subparts of it.", str ) );
}

void TokFromTxt::_on_number() {
    // CALL( number, "..." )
    Node *tok_func = _new_node( Node::Type::Variable, "number" );
    Node *tok_call = _new_node( Node::Type::ParenthesisCall );
    tok_call->add_child( tok_func );

    // seen as a variable
    _append_token( tok_call, { .max_nb_children = 0 } );

    prev_token_is_touching = true;
    pending_new_line = false;
    pending_comma = false;
}

void TokFromTxt::_on_space() {
    prev_token_is_touching = false;
}

bool TokFromTxt::_take_node( Node *token, const TakingInfo &ti ) {
    // find the first item that can be taken (according to ti.left_prio)
    while ( token_stack.size() > 2 && token_stack[ token_stack.size() - 2 ].prio > ti.left_prio && token_stack[ token_stack.size() - 2 ].closing_char == 0 )
        _pop_stack_item();
    StackItem &si = token_stack.back();

    // repl token
    si.token->repl_in_graph_by( token );
    token->add_child( si.token );
    si.token = token;

    // update stack item
    si.min_nb_children = ti.min_nb_children;
    si.max_nb_children = ti.max_nb_children;
    si.closing_char = 0;
    si.prio = ti.right_prio;

    return true;
}

void TokFromTxt::_append_token( Node *token, const AppendingInfo &pti ) {
    // update the stack
    if ( pending_new_line )
        _update_stack_after_newline();
    else if ( pending_comma )
        _update_stack_after_comma();

    // make a call token if last stack item does not take children
    if ( token_stack.back().max_nb_children == 0 ) {
        Node *token_call = _new_node( Node::Type::ParenthesisCall );
        bool took = _take_node( token_call, {
            .max_nb_children = std::numeric_limits<int>::max(),
            .min_nb_children = 0,
            .right_prio = operator_trie->prio_call,
            .left_prio = operator_trie->prio_call,
        } );
        if ( ! took )
            return _error( "found no way to transform the previous token to a call." );
    }

    //
    token_stack.back().token->add_child( token );
    --token_stack.back().min_nb_children;
    --token_stack.back().max_nb_children;

    //
    token_stack << StackItem{
        .token = token,
        .closing_char = 0,
        .on_a_new_line = pending_new_line,
        .newline_size = int( prev_line_beg.size() ),
        .min_nb_children = pti.min_nb_children,
        .max_nb_children = pti.max_nb_children,
        .prio = pti.right_prio
    };
}

END_TL_NAMESPACE

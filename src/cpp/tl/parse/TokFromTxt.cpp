#include "../support/string/va_string.h"
#include "../support/ASSERT.h"
#include "Tok/char_types.h"
#include "TokFromTxt.h"
#include <limits>

#include "../support/P.h"

BEG_TL_NAMESPACE
using namespace Tok;

TokFromTxt::TokFromTxt( Log &log ) : log( log ) {
    _init();
}

void TokFromTxt::parse_txt( StrView content, AString src_url, PI src_off ) {
    curr_src_content = content.begin() - src_off;
    curr_src_url = src_url;
    
    _parse( content.begin(), content.end() );
}

void TokFromTxt::parse_eof() {
    const char eof = 0xFF;
    _parse( &eof, &eof + 1 );
}

Tok::TNode *TokFromTxt::root() {
    return token_stack.front().token;
}

void TokFromTxt::_init() {
    operator_trie = base_operator_trie();

    // pending_...
    prev_token_is_touching = false;
    pending_new_line = true;
    pending_comma = false;

    // other attributes
    // prev_char_value = -1;
    restart_jump = nullptr;
    prev_line_beg.clear();
    pool.clear();

    // 
    TNode *root = _new_node( TNode::Type::ParenthesisCall );
    root->add_child( _new_variable( "module" ) );

    // token_stack
    token_stack = { StackItem{
        .token = root,
        .closing_char = PI32( -1 ),
        .on_a_new_line = true,
        .newline_size = -1,
        .max_nb_children = std::numeric_limits<int>::max(),
        .prio = std::numeric_limits<int>::max(),
    } };
}

void TokFromTxt::_parse( const char *cur, const char *end ) {
    // jump to the right place
    if ( ! restart_jump ) {
        restart_jump = &&on_new_line;
        _start_new_token( cur );
    } else
        curr_tok_ptr_content = cur;

    goto *restart_jump;

    // specific labels =============================================================================================================
    err_unexpected_continuation: _error( "a continuation byte was not expected here (invalid utf8)" ); goto on_eof;
    err_expecting_continuation : _error( "a continuation byte was expected here (invalid utf8)" ); goto on_eof;
    err_unassigned_char        : _error( "unassigned char (invalid utf8)" ); goto on_eof;
    
    on_unauthorized_char       : PE( curr_codepoint ); _error( "unauthorized char" ); goto on_eof;
    on_eof                     : restart_jump = &&on_eof; return;

    // generated code ==============================================================================================================
    // (we use generated code to be able to handle correctly utf8 chars)
    #include "TokFromTxt.gen"
}

void TokFromTxt::_on_opening_paren( const char *end, TNode::Type call_type, const char *func_name, PI32 expected_closing ) {
    if ( prev_token_is_touching ) { // `some_code( a, b )` -> `a` and `b` will be the children of `some_code`
        TNode *token_func = _new_node( TNode::Type::ParenthesisCall );
        bool took = _take_node( token_func, {
            .max_nb_children = std::numeric_limits<int>::max(),
            .min_nb_children = 0,
            .right_prio = operator_trie->prio_call,
            .left_prio = operator_trie->prio_call
        } );
        ASSERT( took );
    } else { // `some_code ( a, b ), c` -> `( a, b )` and `c` will be the children of `some_code`
        TNode *token_func = _new_node( TNode::Type::ParenthesisCall );
        TNode *token_name = _new_variable( func_name );
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

    _start_new_token( end );
}

void TokFromTxt::_on_closing_paren( const char *end, PI32 c ) {
    while ( token_stack.back().closing_char == 0 )
        _pop_stack_item();

    if ( token_stack.back().closing_char != c )
        return _error( va_string( "'$0' has no correspondance", char( c ) ) );

    // 
    StackItem &si = token_stack.back();
    si.max_nb_children = 0;
    si.closing_char = 0;

    //
    prev_token_is_touching = true;
    pending_new_line = false;
    pending_comma = false;

    _start_new_token( end );
}

void TokFromTxt::_on_backslash( const char *end ) {
    TODO;

    _start_new_token( end );
}

void TokFromTxt::_on_semicolon( const char *end ) {
    // gp up
    while ( token_stack.back().closing_char == 0 && token_stack.back().on_a_new_line == false )
        _pop_stack_item();

    // newline => go to parent
    if ( token_stack.back().closing_char == 0 )
        _pop_stack_item();

    prev_token_is_touching = false;
    pending_new_line = false;
    pending_comma = false;

    _start_new_token( end );
}

void TokFromTxt::_on_comma( const char *end ) {
    prev_token_is_touching = false;

    if ( pending_new_line ) {
        _update_stack_after_newline();
        pending_new_line = false;
    }

    if ( pending_comma ) 
        return _error( "a comma just after another comma does not have a defined behavior." );
    pending_comma = true;

    _start_new_token( end );
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
    
TNode *TokFromTxt::_new_node( TNode::Type type ) {
    // create the new token
    TNode *token = pool.create<TNode>();
    // token->string_content = { pool, content };
    token->beg_src_url = curr_tok_src_url;
    token->beg_src_off = curr_tok_src_off;
    token->type = type;
    return token;
}

TNode *TokFromTxt::_new_variable( GString variable_name ) {
    TNode *token = _new_node( TNode::Type::Variable );
    token->variable_ref = variable_name;
    return token;
}

TNode *TokFromTxt::_new_string( StrView content ) {
    TNode *token = _new_node( TNode::Type::String );
    token->string_content = { pool, content };
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

void TokFromTxt::_error( Str msg, Vec<TNode *> tok ) {
    log.msg( msg, {}, Log::Type::ERROR );
}

void TokFromTxt::_start_new_token( const char *cur ) {
    curr_tok_stored_content.clear();
    curr_tok_ptr_content = cur;

    curr_tok_src_off = cur - curr_src_content;
    curr_tok_src_url = curr_src_url;
}

StrView TokFromTxt::_full_tok_content( const char *end ) {
    StrView last_part( curr_tok_ptr_content, end );
    if ( curr_tok_stored_content.empty() )
        return last_part;
    curr_tok_stored_content.append( last_part );
    return curr_tok_stored_content;
}

void TokFromTxt::_on_new_line( const char *end ) {
    StrView tc = _full_tok_content( end );
    while( tc.starts_with( '\n' ) || tc.starts_with( '\r' ) )
        tc.remove_prefix( 1 );
    for( PI i = 0; i < std::min( tc.size(), prev_line_beg.size() ); ++i ) {
        if ( tc[ i ] != prev_line_beg[ i ] ) {
            PE( int( tc[ i ] ), int( prev_line_beg[ i ] ), tc[ i ], prev_line_beg[ i ] );
            _error( "incoherent spacing between the beginning of this line and the previous one" );
            break;
        }
    }

    prev_line_beg = tc;

    prev_token_is_touching = false;
    pending_new_line = true;
    pending_comma = false;

    _start_new_token( end );
}

void TokFromTxt::_on_variable( const char *end ) {
    TNode *tok = _new_variable( _full_tok_content( end ) );
    _append_token( tok, { .max_nb_children = 0 } );

    prev_token_is_touching = true;
    pending_new_line = false;
    pending_comma = false;

    _start_new_token( end );
}

void TokFromTxt::_on_comment( const char *end ) {
    _start_new_token( end );
}

void TokFromTxt::_on_operator( const char *end ) {
    auto use_operator = [&]( OperatorTrie::OperatorData *od, TNode *tok_call ) -> void {
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

    StrView str = _full_tok_content( end );
    while ( OperatorTrie::OperatorData *od = operator_trie->symbol_op( str ) ) {
        TNode *tok_call = _new_node( TNode::Type::ParenthesisCall );
        TNode *tok_func = _new_variable( od->name );
        tok_call->add_child( tok_func );

        use_operator( od, tok_call );

        prev_token_is_touching = true;
        pending_new_line = false;
        pending_comma = false;

        curr_tok_src_off += od->str.size();
        str.remove_prefix( od->str.size() );
    }

    if ( ! str.empty() )
        _error( va_string( "there's no registered operator in '$0' or subparts of it.", str ) );

    _start_new_token( end );
}

void TokFromTxt::_on_number( const char *end ) {
    // CALL( number, "..." )
    TNode *tok_call = _new_node( TNode::Type::ParenthesisCall );
    TNode *tok_sval = _new_string( _full_tok_content( end ) );
    TNode *tok_func = _new_variable( "number" );
    tok_call->add_child( tok_func );
    tok_call->add_child( tok_sval );

    // seen as a variable
    _append_token( tok_call, { .max_nb_children = 0 } );

    prev_token_is_touching = true;
    pending_new_line = false;
    pending_comma = false;

    _start_new_token( end );
}

void TokFromTxt::_on_string( const char *end ) {
    // // CALL( number, "..." )
    // Node *tok_func = _new_node( Node::Type::Variable, "number" );
    // Node *tok_call = _new_node( Node::Type::ParenthesisCall );
    // tok_call->add_child( tok_func );

    // // seen as a variable
    // _append_token( tok_call, { .max_nb_children = 0 } );

    // prev_token_is_touching = true;
    // pending_new_line = false;
    // pending_comma = false;
    TODO;

    _start_new_token( end );
}

void TokFromTxt::_on_space( const char *end ) {
    prev_token_is_touching = false;

    _start_new_token( end );
}

bool TokFromTxt::_take_node( TNode *token, const TakingInfo &ti ) {
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

void TokFromTxt::_append_token( TNode *token, const AppendingInfo &pti ) {
    // update the stack
    if ( pending_new_line )
        _update_stack_after_newline();
    else if ( pending_comma )
        _update_stack_after_comma();

    // make a call token if last stack item does not take children
    if ( token_stack.back().max_nb_children == 0 ) {
        TNode *token_call = _new_node( TNode::Type::ParenthesisCall );
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

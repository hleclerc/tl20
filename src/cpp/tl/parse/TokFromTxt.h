#pragma once

#include "../support/memory/BumpPointerPool.h"
#include "../support/string/StrRef.h"
#include "../support/containers/Vec.h"
#include "../support/log/Log.h"

#include "Tok/OperatorTrie.h"
#include "Tok/StackItem.h"

BEG_TL_NAMESPACE

/** 
    Transform a text to a token tree.


*/
class TokFromTxt {
public:
    /**/               TokFromTxt                 ( Log &log );

    void               parse_txt                  ( StrView content, StrRef src_url, PI src_off = 0 ); ///< `src_off` = offset in content of `src_url`
    void               parse_eof                  ();
    
    Tok::Node*         root                       (); ///<
                     
private:      
    struct             AppendingInfo              { int min_nb_children, max_nb_children; int right_prio = 100; };
    struct             TakingInfo                 { int min_nb_children, max_nb_children; int right_prio = 100, left_prio = 100; };
    using              StackItem                  = Tok::StackItem;
        
    enum {             eof                        = -1 }; ///< internal codes for _parse
                       
    void               _on_opening_paren          ( Tok::Node::Type call_type, const char *func_name, char expected_closing );
    void               _on_closing_paren          ( char c );
    void               _on_semicolon              ();
    void               _on_new_line               ( const char *end );
    void               _on_variable               ( const char *end );
    void               _on_operator               ( const char *end );
    void               _on_comment                ( const char *end );
    void               _on_number                 ( const char *end );
    void               _on_string                 ( const char *end );
    void               _on_space                  ( const char *end );
    void               _on_comma                  ();
    
    void               _update_stack_after_newline(); ///< update stack position after a new line
    void               _update_stack_after_comma  (); ///< update stack position after a comma
    StrView            _full_tok_content          ( const char *end ); ///< string content of the current token
    void               _start_new_token           ( const char *cur ); ///< 
    void               _pop_stack_item            ();
    void               _append_token              ( Tok::Node *token, const AppendingInfo &ai );
    bool               _take_node                 ( Tok::Node *token, const TakingInfo &ti );
    void               _error                     ( Str msg, Vec<Tok::Node *> tok = {} );
    void               _parse                     ( const char *cur, const char *end );
    void               _init                      ();

    Tok::Node*         _new_variable              ( StrRef variable_name );
    Tok::Node*         _new_string                ( StrView content );
    Tok::Node*         _new_node                  ( Tok::Node::Type type );
    
    Tok::OperatorTrie* operator_trie;             ///<
    
    bool               prev_token_is_touching;    ///<
    bool               pending_new_line;          ///<
    bool               pending_comma;             ///<
      
    Str                curr_tok_stored_content;   ///< content that comes from a previous buffer
    const char*        curr_tok_ptr_content;      ///< beginning of the current token content that comes from the current buffer
    PI                 curr_tok_src_off;          ///< off of the beginning of the current tok
    StrRef             curr_tok_src_url;          ///< url of the beginning of the current tok

    const char*        curr_src_content;          ///< beginning of the current source content
    StrRef             curr_src_url;              ///<

    PI32               curr_codepoint;            ///< used for multi byte utf8

    Str                prev_line_beg;             ///<
    void*              restart_jump;              ///< where to go at the beginning of `parse`
    Vec<StackItem>     token_stack;               ///<
    BumpPointerPool    pool;                      ///<
    Log&               log;                       ///<
};

END_TL_NAMESPACE

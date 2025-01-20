#pragma once

#include "../support/memory/BumpPointerPool.h"
#include "../support/containers/Vec.h"
#include "AstWriter.h"
#include "TlToken.h"
#include "tl/support/Displayer.h"

BEG_TL_NAMESPACE

/** 
    Make Ast content from some text

    f a + b


*/
class TlParser {
public:
    /**/            TlParser              ();

    void            display               ( Displayer &ds ) const;
    void            parse                 ( StrView content, PI src_off, AstWriterStr src_url );
    void            dump                  ( AstWriter &writer );
               
private:
    struct          StackItem             { TlToken *token; int closing_char; bool on_a_new_line; int newline_size; };

    enum {          eof                   = -1 }; ///< internal codes for _parse
               
    void            _on_opening_paren     ( TlToken::Type call_type, char expected_closing );
    void            _on_closing_paren     ( char c );
    void            _on_semicolon         ();
    void            _on_new_line          ( AstWriterStr src_url, PI src_off );
    void            _on_variable          ( AstWriterStr src_url, PI src_off );
    void            _on_operator          ( AstWriterStr src_url, PI src_off );
    void            _on_number            ( AstWriterStr src_url, PI src_off );
    void            _on_space             ();
    void            _on_comma             ();

    void            _update_stack_after_nl();
    void            _add_child_to         ( TlToken *parent, TlToken *child );
    void            _push_token           ( TlToken::Type type, AstWriterStr src_url, PI src_off );
    void            _error                ( Str msg, AstWriterStr src_url, PI src_off );
    void            _parse                ( int c, const char *nxt, const char *beg, const char *end, AstWriterStr src_url );
    void            _init                 ();
      
    AstWriterStr    curr_tok_src_url;     ///<
    PI              curr_tok_src_off;     ///<
    Str             curr_tok_content;     ///<
    int             prev_char_value;      ///< 
    void*           restart_jump;         ///<

    bool            just_seen_a_new_line; ///<
    bool            just_seen_a_space;    ///<
    Str             prev_line_beg;        ///<

    Vec<StackItem>  token_stack;          ///<
    BumpPointerPool pool;                 ///<
};

END_TL_NAMESPACE

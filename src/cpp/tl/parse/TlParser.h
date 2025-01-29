#pragma once

#include "../support/memory/BumpPointerPool.h"
#include "../support/containers/Vec.h"
#include "TlParserStackItem.h"
#include "OperatorTrie.h"
#include "AstWriter.h"
#include "tl/parse/TlToken.h"

BEG_TL_NAMESPACE

/** 
    Make Ast content from some text

    f a + b


*/
class TlParser {
public:
    /**/            TlParser               ();
 
    void            display                ( Displayer &ds ) const;
    void            parse                  ( StrView content, PI src_off, AstWriterStr src_url );
    void            dump                   ( AstWriter &writer );
                
private: 
    using           StackItem              = TlParserStackItem;
    using           SrcRef                 = TlToken::SrcRef;
 
    enum {          eof                    = -1 }; ///< internal codes for _parse
                
    void            _on_opening_paren      ( TlToken::Type call_type, char expected_closing );
    void            _on_closing_paren      ( char c );
    void            _on_semicolon          ();
    void            _on_new_line           ();
    void            _on_variable           ();
    void            _on_operator           ();
    void            _on_number             ();
    void            _on_space              ();
    void            _on_comma              ();
 
    void            _update_stack_after_nl ();
    void            _add_child_to          ( TlToken *parent, TlToken *child );
    void            _push_token            ( TlToken::Type type, StrView content, int right_prio );
    void            _take_left             ( TlToken::Type type, StrView content, int right_prio, int left_prio );
    void            _error                 ( Str msg );
    void            _parse                 ( int c, const char *nxt, const char *beg, const char *end, AstWriterStr src_url );
    void            _init                  ();
       
    Vec<SrcRef>     curr_tok_src_refs;     ///<
    Str             curr_tok_content;      ///<
    int             prev_char_value;       ///< used notably for cnt_number where '+' or '-' can be continuing chars if preceded by 'e' or 'E'
    void*           restart_jump;          ///< where to go at the beginning of `parse`

    bool            just_seen_a_semicolon; ///<
    bool            just_seen_a_new_line;  ///<
    bool            just_seen_a_comma;     ///<
    bool            just_seen_a_space;     ///<
    Str             prev_line_beg;         ///<
 
    OperatorTrie*   operator_trie;         ///<
    Vec<StackItem>  token_stack;           ///<
    BumpPointerPool pool;                  ///<
};

END_TL_NAMESPACE

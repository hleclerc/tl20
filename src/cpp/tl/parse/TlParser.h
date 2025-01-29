#pragma once

#include "../support/memory/BumpPointerPool.h"
#include "../support/containers/Vec.h"
#include "TlParserStackItem.h"
#include "OperatorTrie_Tl.h"
#include "AstWriter.h"
#include <limits>

BEG_TL_NAMESPACE

/** 
    Make Ast content from some text

    f a + b


*/
class TlParser {
public:
    /**/             TlParser                   ();
      
    void             parse                      ( StrView content, PI src_off, AstWriterStr src_url );
    void             dump                       ( AstWriter &writer );
    
    Str              condensed                  () const;
    void             display                    ( Displayer &ds ) const;
                     
private:      
    struct           AppendingInfo              { int min_nb_children, max_nb_children; int right_prio = 100; };
    struct           TakingInfo                 { int min_nb_children, max_nb_children; int right_prio = 100, left_prio = 100; };
    using            StackItem                  = TlParserStackItem;
    using            SrcRef                     = TlToken::SrcRef;
      
    enum {           eof                        = -1 }; ///< internal codes for _parse
                     
    void             _on_opening_paren          ( TlToken::Type call_type, const char *func_name, char expected_closing );
    void             _on_closing_paren          ( char c );
    void             _on_semicolon              ();
    void             _on_new_line               ();
    void             _on_variable               ();
    void             _on_operator               ();
    void             _on_number                 ();
    void             _on_space                  ();
    void             _on_comma                  ();
  
    void             _update_stack_after_newline(); ///< update stack position after a new line
    void             _update_stack_after_comma  (); ///< update stack position after a comma
    void             _pop_stack_item            ();
    void             _append_token              ( TlToken *token, const AppendingInfo &ai );
    bool             _take_token                ( TlToken *token, const TakingInfo &ti );
    TlToken*         _new_token                 ( TlToken::Type type, StrView content = {} );
    void             _error                     ( Str msg, Vec<TlToken *> tok = {} );
    void             _parse                     ( int c, const char *nxt, const char *beg, const char *end, AstWriterStr src_url );
    void             _init                      ();
    
    bool             local_operator_trie;       ///<
    OperatorTrie_Tl* operator_trie;             ///<
    
    bool             prev_token_is_touching;    ///<
    bool             pending_new_line;          ///<
    bool             pending_comma;             ///<
    
    Vec<SrcRef>      curr_tok_src_refs;         ///<
    Str              curr_tok_content;          ///<
    int              prev_char_value;           ///< used notably for cnt_number where '+' or '-' can be continuing chars if preceded by 'e' or 'E'
    Str              prev_line_beg;             ///<
    void*            restart_jump;              ///< where to go at the beginning of `parse`
    Vec<StackItem>   token_stack;               ///<
    BumpPointerPool  pool;                      ///<
};

END_TL_NAMESPACE

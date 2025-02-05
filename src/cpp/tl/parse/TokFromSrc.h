#pragma once

#include "../support/memory/BumpPointerPool.h"
#include "../support/containers/Vec.h"
#include "../support/log/Log.h"

#include "tl/parse/Ast/StringRef.h"
#include "Tok/OperatorTrie.h"
#include "Tok/StackItem.h"

BEG_TL_NAMESPACE

/** 
    Make Ast content from some text

    f a + b


*/
class TokFromSrc {
public:
    /**/               TokFromSrc                 ( Log &log );
        
    void               parse                      ( StrView content, PI src_off, Ast::StringRef src_url, bool write_eof = true );
    Tok::Node*         root                       (); ///< (operator module, ...)
      
    Str                condensed                  () const; ///< condensed human readable representation (for debug purpose). Removes `operator module` as first child
    void               display                    ( Displayer &ds ) const;
                     
private:      
    struct             AppendingInfo              { int min_nb_children, max_nb_children; int right_prio = 100; };
    struct             TakingInfo                 { int min_nb_children, max_nb_children; int right_prio = 100, left_prio = 100; };
    using              StackItem                  = Tok::StackItem;
    using              SrcRef                     = Tok::Node::SrcRef;
        
    enum {             eof                        = -1 }; ///< internal codes for _parse
                       
    void               _on_opening_paren          ( Tok::Node::Type call_type, const char *func_name, char expected_closing );
    void               _on_closing_paren          ( char c );
    void               _on_semicolon              ();
    void               _on_new_line               ();
    void               _on_variable               ();
    void               _on_operator               ();
    void               _on_number                 ();
    void               _on_space                  ();
    void               _on_comma                  ();
    
    void               _update_stack_after_newline(); ///< update stack position after a new line
    void               _update_stack_after_comma  (); ///< update stack position after a comma
    void               _pop_stack_item            ();
    void               _append_token              ( Tok::Node *token, const AppendingInfo &ai );
    bool               _take_node                 ( Tok::Node *token, const TakingInfo &ti );
    Tok::Node*         _new_node                  ( Tok::Node::Type type, StrView content = {} );
    void               _error                     ( Str msg, Vec<Tok::Node *> tok = {} );
    void               _parse                     ( int c, const char *nxt, const char *beg, const char *end, Ast::StringRef src_url );
    void               _init                      ();
    
    bool               local_operator_trie;       ///<
    Tok::OperatorTrie* operator_trie;             ///<
    
    bool               prev_token_is_touching;    ///<
    bool               pending_new_line;          ///<
    bool               pending_comma;             ///<
      
    Vec<SrcRef>        curr_tok_src_refs;         ///<
    Str                curr_tok_content;          ///<
    int                prev_char_value;           ///< used notably for cnt_number where '+' or '-' can be continuing chars if preceded by 'e' or 'E'
    Str                prev_line_beg;             ///<
    void*              restart_jump;              ///< where to go at the beginning of `parse`
    Vec<StackItem>     token_stack;               ///<
    BumpPointerPool    pool;                      ///<
    Log&               log;                       ///<
};

END_TL_NAMESPACE

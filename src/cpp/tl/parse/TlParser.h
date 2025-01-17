#pragma once

#include "AstWriter.h"

BEG_TL_NAMESPACE

/** 
    Make Ast content from some text

    f a + b


*/
class TlParser {
public:
    /**/         TlParser      ();
    
    void         parse         ( StrView content, PI src_off, AstWriterStr src_url );
    void         dump          ( AstWriter &writer );
    
private:      
    enum {       eof  = -1 }; ///< internal codes for _parse
    
    void         _on_new_line  ();
    void         _on_variable  ();
    void         _on_operator  ();
    void         _on_number    ();
    void         _on_space     ();

    void         _parse        ( int c, const char *nxt, const char *beg, const char *end, AstWriterStr src_url );
    void         _init         ();

    AstWriterStr curr_tok_src_url; ///<
    PI           curr_tok_src_off; ///<
    Str          curr_tok_content; ///<
    int          prev_char_value;  ///< 
    void*        restart_jump;     ///<
};

END_TL_NAMESPACE

#pragma once

#include "AstWriterStr.h"

BEG_TL_NAMESPACE

/** 
*/
class TlToken {
public:
    enum class   Type        { Root, Variable, Number, ParenthesisCall };

    AstWriterStr src_url;
    PI           src_off;
    PI           src_len;
    Str          content;
    Type         type;
    
    TlToken*     first_child = nullptr;
    TlToken*     parent      = nullptr;
    TlToken*     prev        = nullptr;
    TlToken*     next        = nullptr;
};

END_TL_NAMESPACE

#pragma once

#include "../support/memory/PoolVec.h"
#include "../support/Displayer.h"
#include "AstWriterStr.h"

BEG_TL_NAMESPACE

/** 
*/
class TlToken {
public:
    enum class      CallType    { None, ParenthesisCall, BraceCall, BracketCall };
    enum class      Type        { Root, Variable, Number, ParenthesisCall, BraceCall, BracketCall };
    struct          SrcRef      { AstWriterStr url; PI beg, end; };
   
    void            display     ( Displayer &ds ) const;

    PoolVec<SrcRef> src_refs;
    Str             content;
    Type            type;
   
    CallType        call_type;  ///<
   
    TlToken*        first_child = nullptr;
    TlToken*        last_child  = nullptr;
    TlToken*        parent      = nullptr;
    TlToken*        prev        = nullptr;
    TlToken*        next        = nullptr;
};

END_TL_NAMESPACE

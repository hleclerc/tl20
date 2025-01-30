#pragma once

#include "../Displayer.h"
#include "Log.h"

/**
*/
class TestingLog : public Log {
public:
    struct         Message    { Str msg; MsgId id; LogProvenance pr; void display( Displayer &sr ) const; };
    using          VM         = Vec<Message>;

    void           display    ( Displayer &sr ) const;
    virtual void   add_sub_msg( MsgId id, StrView msg, const LogProvenance &pr = {} ) override;
    virtual MsgId  beg_msg    ( StrView msg, const LogProvenance &pr = {}, Type type = Type::ERROR ) override;
    virtual void   end_msg    ( MsgId id ) override;

    VM             messages;
};

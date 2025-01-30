#pragma once

#include "LogProvenance.h"

/**
*/
class Log {
public:
    enum class         Type       { ERROR, WARNING, INFO };
    using              MsgId      = unsigned;

    virtual           ~Log        ();

    virtual MsgId      beg_msg    ( StrView msg, const LogProvenance &pr = {}, Type type = Type::ERROR ) = 0;
    virtual void       end_msg    ( MsgId id ) = 0;

    void               msg        ( StrView msg, const LogProvenance &pr = {}, Type type = Type::ERROR );

    virtual void       add_sub_msg( MsgId id, StrView msg, const LogProvenance &pr = {} ) = 0;

    static const char *type_string( Type type );
};

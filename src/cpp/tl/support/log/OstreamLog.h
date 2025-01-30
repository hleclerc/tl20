#pragma once

#include "StyleOfTextLog.h"
#include "Log.h"

#include <ostream>

/**
*/
class OstreamLog : public Log {
public:
    /**/           OstreamLog ( std::ostream *os = nullptr, const StyleOfTextLog &style = {} );

    virtual void   add_sub_msg( MsgId id, StrView msg, const LogProvenance &pr = {} ) override;
    virtual MsgId  beg_msg    ( StrView msg, const LogProvenance &pr = {}, Type type = Type::ERROR ) override;
    virtual void   end_msg    ( MsgId id ) override;

    StyleOfTextLog style;
    std::ostream*  os;
};

extern OstreamLog *ostream_log;

#include "LogProvenance.h"
#include "OstreamLog.h"
#include <iostream>

static OstreamLog _ostream_log;
OstreamLog *ostream_log = &_ostream_log;

OstreamLog::OstreamLog( std::ostream *os, const StyleOfTextLog &style ) : style( style ), os( os ? os : &std::cerr ) {
}

void OstreamLog::add_sub_msg( MsgId /*id*/, StrView msg, const LogProvenance &pr ) {
    pr.display_text_header( *os, style );
    *os << msg;
    pr.display_inline_extract( *os, style );

}

Log::MsgId OstreamLog::beg_msg( StrView msg, const LogProvenance &pr, Type type ) {
    pr.display_text_header( *os, style );
    *os << type_string( type ) << ": ";
    *os << msg;
    pr.display_text_extract( *os, style, true );
    *os << "\n";
    return 0;
}

void OstreamLog::end_msg( MsgId /*id*/ ) {
}

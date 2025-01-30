#include "TestingLog.h"

void TestingLog::add_sub_msg( MsgId /*id*/, StrView /*msg*/, const LogProvenance &/*pr*/ ) {
}

Log::MsgId TestingLog::beg_msg( StrView msg, const LogProvenance &pr, Type /*type*/ ) {
    messages.push_back( Message{ .msg = Str( msg ), .id  = 0, .pr = pr } );
    return 0;
}

void TestingLog::end_msg( MsgId /*id*/ ) {
}

void TestingLog::display( Displayer &sr ) const {
    sr << messages;
}

void TestingLog::Message::display( Displayer &sr ) const {
    sr << pr.offsets[ 0 ];
}


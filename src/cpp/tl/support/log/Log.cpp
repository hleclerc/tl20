#include "../TODO.h"
#include "Log.h"

Log::~Log() {
}

void Log::msg( StrView msg, const LogProvenance &pr, Type type ) {
    end_msg( beg_msg( msg, pr, type ) );
}

const char *Log::type_string( Type type ) {
    switch( type ) {
    case Type::WARNING: return "WARNING";
    case Type::ERROR: return "ERROR";
    case Type::INFO: return "INFO";
    default: TODO;
    }
    return nullptr;
}


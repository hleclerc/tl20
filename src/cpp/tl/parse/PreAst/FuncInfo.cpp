#include "FuncInfo.h"

BEG_TL_NAMESPACE
namespace PreAst {

FuncInfo::FuncInfo() {
    arg_types[ "operator =>" ] = { ArgType::ArgDecl, ArgType::DelayedBlock };
    arg_types[ "class"       ] = { ArgType::VarDecl, ArgType::DelayedBlock };
    arg_types[ "def"         ] = { ArgType::VarDecl, ArgType::DelayedBlock };

    arg_types[ "info"        ] = { ArgType::ImmediateBlock };
    arg_types[ "and"         ] = { ArgType::ImmediateBlock };
    arg_types[ "or"          ] = { ArgType::ImmediateBlock };

    arg_types[ "while"       ] = { ArgType::Value  , ArgType::ImmediateBlock };
    arg_types[ "for"         ] = { ArgType::InDecl , ArgType::ImmediateBlock };
    arg_types[ "if"          ] = { ArgType::Value  , ArgType::ImmediateBlock };
}

FuncInfo *base_func_info() {
    static FuncInfo res;
    return &res;
}

} // namespace PreAst
END_TL_NAMESPACE

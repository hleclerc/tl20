#include "FuncType.h"

BEG_TL_NAMESPACE
namespace PreAst {

struct BaseFuncMap {
    BaseFuncMap() {
        func_map[ "operator :=" ] = FuncType::Assignation;
        func_map[ "operator =>" ] = FuncType::Lambda;
        func_map[ "class"       ] = FuncType::Class;
        func_map[ "def"         ] = FuncType::Def;

        func_map[ "info"        ] = FuncType::ImmediateBlock;
        func_map[ "and"         ] = FuncType::ImmediateBlock;
        func_map[ "or"          ] = FuncType::ImmediateBlock;

        func_map[ "while"       ] = FuncType::Test;
        func_map[ "if"          ] = FuncType::Test;

        func_map[ "for"         ] = FuncType::For;
    }

    std::map<Str,FuncType> func_map;
};

std::map<Str,FuncType> *base_func_map() {
    static BaseFuncMap bfm;
    return &bfm.func_map;
}

} // namespace PreAst
END_TL_NAMESPACE

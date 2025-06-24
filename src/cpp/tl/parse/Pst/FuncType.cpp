#include "FuncType.h"

BEG_TL_NAMESPACE
namespace Pst {

struct BaseFuncMap {
    BaseFuncMap() {
        func_map[ "operator module" ] = FuncType::Module;

        func_map[ "operator :="     ] = FuncType::Assignation;
        func_map[ "operator =>"     ] = FuncType::Lambda;
        func_map[ "class"           ] = FuncType::Class;
        func_map[ "def"             ] = FuncType::Def;
    
        func_map[ "info"            ] = FuncType::ImmediateBlock;
        func_map[ "and"             ] = FuncType::ImmediateBlock;
        func_map[ "or"              ] = FuncType::ImmediateBlock;
    
        func_map[ "while"           ] = FuncType::Test;
        func_map[ "if"              ] = FuncType::Test;
    
        func_map[ "for"             ] = FuncType::For;
    }

    std::map<GString,FuncType> func_map;
};

std::map<GString,FuncType> *base_func_map() {
    static BaseFuncMap bfm;
    return &bfm.func_map;
}

} // namespace Pst
END_TL_NAMESPACE

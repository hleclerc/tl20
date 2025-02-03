#pragma once

#include "../../support/containers/Vec.h"
#include <map>

BEG_TL_NAMESPACE
namespace PreAst {

/** 

*/
class FuncInfo {
public:
    enum class  ArgType  {
                ImmediateBlock, 
                DelayedBlock, 
                VarDecl,         /*ex: first arg of `def f(a,b)` */ 
                ArgDecl,         /*ex: `a => ...`, for a in L, ...*/ 
                InDecl,          /*for ... in ...*/ 
                Value 
    };
    using       ArgMap   = std::map<Str,Vec<ArgType>>;

    /**/        FuncInfo();

    ArgMap      arg_types;
};

FuncInfo *base_func_info();


} // namespace PreAst
END_TL_NAMESPACE

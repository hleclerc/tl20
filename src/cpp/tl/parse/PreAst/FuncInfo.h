#pragma once

#include "../../support/containers/Vec.h"
#include <map>

BEG_TL_NAMESPACE
namespace PreAst {

/** 

*/
class FuncInfo {
public:
    enum class  Behavior  {
        ImmediateBlock, // as in `info a, ...` or `a or ...` where `a, ...` is sent as a block, expected to be fully or partially executed without delay (meaning that it's not possible to catch variable declared after that)
        Assignation   , // as in `a := ...` where `...` is by default an immediate block, unless the lhs if a function (as in `a( b, c ) := ...`, where `...` is a delayed block)
        Lambda        , // as in `a => ...` where `a` is an arg decl and`...` is (always) a delayed block
        Class         , //`class f(...) ...` where `...` is a delayed block with "attributes"
        Test          , // as in `if` or `while ...` with a condition and an immediate block, potentially with an else
        Def           , // `def f( a, b ), ...` where `...` is a delayed block
        For           , // as in `for ... in ..., ...`
    };
    using       ArgMap   = std::map<Str,Vec<Behavior>>;

    /**/        FuncInfo();

    ArgMap      arg_types;
};

FuncInfo *base_func_info();


} // namespace PreAst
END_TL_NAMESPACE

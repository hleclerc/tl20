#pragma once

#include "Variable.h"
#include "FuncInfo.h"

BEG_TL_NAMESPACE
namespace PreAst {

/** 

*/
class Scope {
public:
    enum class          Type        { Immediate, Delayed, Class };

    /**/                Scope       ( Scope *parent, Type type = Type::Immediate );
    void                display     ( Displayer &ds ) const;
    // virtual void     write       ( AstWriter &aw ) const;
    
    PI                  nb_variables_in_parents;
    FuncInfo*           func_info;
    Vec<Variable>       variables;
    Scope*              parent;
    Type                type;
};

} // namespace PreAst
END_TL_NAMESPACE

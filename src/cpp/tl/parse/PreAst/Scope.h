#pragma once

#include "../../support/containers/Vec.h"
#include "../../support/Displayer.h"
#include "FuncType.h"

BEG_TL_NAMESPACE
namespace PreAst {

/** 

*/
class Scope {
public:
    using               FuncMap     = std::map<Str,FuncType>;
    enum class          Type        { Immediate, Delayed, Class };

    /**/                Scope       ( Scope *parent = nullptr, Type type = Type::Immediate );
    void                display     ( Displayer &ds ) const;
    // virtual void     write       ( AstWriter &aw ) const;
    
    PI                  nb_variables_in_parents_during_creation;
    Vec<Str>            variable_names; ///< name of created variables
    Vec<PI>             references;     ///< variable coming from the parent scope
    FuncMap*            func_map;
    Scope*              parent;
    Type                type;
};

} // namespace PreAst
END_TL_NAMESPACE

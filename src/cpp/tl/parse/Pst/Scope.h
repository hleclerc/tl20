#pragma once

#include "../../support/containers/Vec.h"
#include "../../support/string/GString.h"
#include "../../support/Displayer.h"
#include "FuncType.h"

BEG_TL_NAMESPACE
namespace Pst {

/** 

*/
class Scope {
public:
    using               FuncMap     = std::map<GString,FuncType>;
    enum class          Type        { Immediate, Delayed, Class };

    /**/                Scope       ( Scope *parent = nullptr, Type type = Type::Immediate );

    Str                 condensed   () const;
    void                display     ( Displayer &dp ) const;
    // virtual void     write       ( Ast &ast ) const;
    
    PI                  nb_variables_in_parents_during_creation;
    Vec<GString>*       global_variables; ///< if variable if not found, store the name here (+ in variable_names)
    Vec<GString>        variable_names;   ///< name of created variables
    Vec<PI>             references;       ///< variable coming from the parent scope
    FuncMap*            func_map;
    Scope*              parent;
    Type                type;
};

} // namespace Pst
END_TL_NAMESPACE

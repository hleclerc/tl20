#include "Scope.h"

BEG_TL_NAMESPACE
namespace Pst {

Scope::Scope( Scope *parent, Type type ) : parent( parent ), type( type ) {
    nb_variables_in_parents_during_creation = parent ? parent->variable_names.size() : 0;
    func_map = parent ? parent->func_map : nullptr;
    global_variables = nullptr;
}

Str Scope::condensed() const {
    return "";
}

void Scope::display( Displayer &dp ) const {
    DS_OBJECT( Scope, variable_names, references );
}

} // namespace Pst
END_TL_NAMESPACE

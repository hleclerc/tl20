#include "Scope.h"

BEG_TL_NAMESPACE
namespace PreAst {

Scope::Scope( Scope *parent, Type type ) : parent( parent ), type( type ) {
    nb_variables_in_parents_during_creation = parent ? parent->variable_names.size() : 0;
    func_map = parent ? parent->func_map : nullptr;
}

void Scope::display( Displayer &ds ) const {
    DS_OBJECT( Scope, variable_names, references );
}

} // namespace PreAst
END_TL_NAMESPACE

#include "Scope.h"

BEG_TL_NAMESPACE
namespace PreAst {

Scope::Scope( Scope *parent, Type type ) : parent( parent ), type( type ) {
    nb_variables_in_parents = parent ? parent->variables.size() : 0;
    func_info = parent ? parent->func_info : nullptr;
}

void Scope::display( Displayer &ds ) const {
    ds << variables;
}

} // namespace PreAst
END_TL_NAMESPACE

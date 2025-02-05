#include "Node_Variable.h"
#include "Scope.h"

BEG_TL_NAMESPACE
namespace Pst {

Node_Variable::Node_Variable( Tok::Node *token, Scope *scope, const Str &name ) : Node( token ), scope( scope ), name( name ) {
    nb_variables_in_scope_during_construction = scope->variable_names.size();
}

void Node_Variable::display( Displayer &ds ) const {
    ds << name;
}

void Node_Variable::write( Ast::Writer &aw ) const {
    TODO;
}

} // namespace Pst
END_TL_NAMESPACE

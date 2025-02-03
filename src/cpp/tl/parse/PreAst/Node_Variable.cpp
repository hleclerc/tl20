#include "Node_Variable.h"
#include "Scope.h"

BEG_TL_NAMESPACE
namespace PreAst {

Node_Variable::Node_Variable( TlToken *token, Scope *scope, const Str &name ) : Node( token ), scope( scope ), name( name ) {
    nb_variables_in_scope = scope->variables.size();
}

void Node_Variable::display( Displayer &ds ) const {
    ds << name;
}

void Node_Variable::write( AstWriter &aw ) const {
    TODO;
}

} // namespace PreAst
END_TL_NAMESPACE

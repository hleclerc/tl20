#include "Node_Call.h"
#include "Scope.h"

BEG_TL_NAMESPACE
namespace PreAst {

Node_Call::Node_Call( TlToken *token, Scope *parent_scope ) : Node( token ), scope( parent_scope ) {
}

void Node_Call::display( Displayer &ds ) const {
    DS_OBJECT( Node_Call, scope, func, args );
}

void Node_Call::write( AstWriter &aw ) const {
    TODO;
}

} // namespace PreAst
END_TL_NAMESPACE

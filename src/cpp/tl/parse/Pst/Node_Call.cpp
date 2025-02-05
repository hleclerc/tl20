#include "Node_Call.h"
#include "Scope.h"

BEG_TL_NAMESPACE
namespace Pst {

Node_Call::Node_Call( Tok::Node *token, Scope *parent_scope ) : Node( token ), scope( parent_scope ) {
}

void Node_Call::display( Displayer &ds ) const {
    DS_OBJECT( Node_Call, scope, func, args );
}

void Node_Call::write( Ast::Writer &aw ) const {
    TODO;
}

} // namespace Pst
END_TL_NAMESPACE

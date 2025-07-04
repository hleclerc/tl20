#include "Node_Call.h"
#include "Scope.h"

BEG_TL_NAMESPACE
namespace Pst {

Node_Call::Node_Call( Tok::TNode *token, Scope *parent_scope ) : PNode( token ), scope( parent_scope ) {
}

void Node_Call::display( Displayer &dp ) const {
    DS_OBJECT( Node_Call, scope, func, args );
}

void Node_Call::write( Ast &ast ) const {
    TODO;
}

} // namespace Pst
END_TL_NAMESPACE

#include "Node_Block.h"

BEG_TL_NAMESPACE
namespace PreAst {

Node_Block::Node_Block( TlToken *token, Scope *parent_scope, Scope::Type scope_type ) : Node( token ), scope( parent_scope, scope_type ) {
}

void Node_Block::display( Displayer &ds ) const {
    ds << nodes;
}

void Node_Block::write( AstWriter &aw ) const {
    TODO;
}

} // namespace PreAst
END_TL_NAMESPACE

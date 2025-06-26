#include "Node_Block.h"

BEG_TL_NAMESPACE
namespace Pst {

Node_Block::Node_Block( Tok::TNode *token, Scope *parent_scope, Scope::Type scope_type ) : Node( token ), block( parent_scope, scope_type ) {
}

void Node_Block::display( Displayer &dp ) const {
    ds << block;
}

void Node_Block::write( Ast &ast ) const {
    TODO;
}

} // namespace Pst
END_TL_NAMESPACE

#include "Block.h"

BEG_TL_NAMESPACE
namespace PreAst {

Block::Block( TlToken *token, Scope *parent_scope, Scope::Type scope_type ) : Node( token ), scope( parent_scope, scope_type ) {
}

void Block::display( Displayer &ds ) const {
    ds << nodes;
}

void Block::write( AstWriter &aw ) const {
    TODO;
}

} // namespace PreAst
END_TL_NAMESPACE

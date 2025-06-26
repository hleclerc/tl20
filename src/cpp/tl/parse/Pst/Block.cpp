#include "Block.h"

BEG_TL_NAMESPACE
namespace Pst {

Block::Block( Scope *parent_scope, Scope::Type scope_type ) : scope( parent_scope, scope_type ) {
}

void Block::display( Displayer &dp ) const {
    DS_OBJECT( Block, scope, nodes );
}

void Block::write( Ast &ast ) const {
    TODO;
}

} // namespace Pst
END_TL_NAMESPACE

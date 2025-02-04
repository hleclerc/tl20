#include "Block.h"

BEG_TL_NAMESPACE
namespace PreAst {

void Block::display( Displayer &ds ) const {
    DS_OBJECT( Block, scope, nodes );
}

void Block::write( AstWriter &aw ) const {
    TODO;
}

} // namespace PreAst
END_TL_NAMESPACE

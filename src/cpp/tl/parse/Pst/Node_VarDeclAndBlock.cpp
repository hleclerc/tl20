#include "Node_VarDeclAndBlock.h"

BEG_TL_NAMESPACE
namespace Pst {

void Node_VarDeclAndBlock::display( Displayer &ds ) const {
    DS_OBJECT( Node_VarAndBlock, var_decl, block );
}

void Node_VarDeclAndBlock::write( Ast::Writer &aw ) const {
    TODO;
}

} // namespace Pst
END_TL_NAMESPACE

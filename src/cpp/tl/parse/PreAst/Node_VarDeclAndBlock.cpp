#include "Node_VarDeclAndBlock.h"

BEG_TL_NAMESPACE
namespace PreAst {

void Node_VarDeclAndBlock::display( Displayer &ds ) const {
    DS_OBJECT( Node_VarAndBlock, var_decl, block );
}

void Node_VarDeclAndBlock::write( AstWriter &aw ) const {
    TODO;
}

} // namespace PreAst
END_TL_NAMESPACE

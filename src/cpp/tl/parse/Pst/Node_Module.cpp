#include "Node_Module.h"

BEG_TL_NAMESPACE
namespace Pst {

Node_Module::Node_Module( Tok::TNode *token ) : PNode( token ) {
}

void Node_Module::display( Displayer &dp ) const {
    DS_OBJECT( Node_Module, global_variables, block );
}

void Node_Module::write( Ast &ast ) const {
    TODO;
}

} // namespace Pst
END_TL_NAMESPACE

#include "Node_Module.h"
#include "tl/support/Displayer.h"

BEG_TL_NAMESPACE
namespace Pst {

Node_Module::Node_Module( Tok::Node *token ) : Node( token ) {
}

void Node_Module::display( Displayer &ds ) const {
    DS_OBJECT( Node_Module, global_variables, block );
}

void Node_Module::write( Ast::Writer &aw ) const {
    TODO;
}

} // namespace Pst
END_TL_NAMESPACE

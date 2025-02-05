#include "Node_String.h"

BEG_TL_NAMESPACE
namespace Pst {

Node_String::Node_String( Tok::Node *token, const Str &content ) : Node( token ), content( content ) {
}

void Node_String::display( Displayer &ds ) const {
    ds << content;
}

void Node_String::write( Ast::Writer &aw ) const {
    TODO;
}

} // namespace Pst
END_TL_NAMESPACE

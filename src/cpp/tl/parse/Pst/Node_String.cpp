#include "Node_String.h"

BEG_TL_NAMESPACE
namespace Pst {

Node_String::Node_String( Tok::TNode *token, const Str &content ) : PNode( token ), content( content ) {
}

void Node_String::display( Displayer &dp ) const {
    dp << content;
}

void Node_String::write( Ast &ast ) const {
    TODO;
}

} // namespace Pst
END_TL_NAMESPACE

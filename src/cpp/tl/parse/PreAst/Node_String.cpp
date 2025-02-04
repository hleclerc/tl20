#include "Node_String.h"

BEG_TL_NAMESPACE
namespace PreAst {

Node_String::Node_String( TlToken *token, const Str &content ) : Node( token ), content( content ) {
}

void Node_String::display( Displayer &ds ) const {
    ds << content;
}

void Node_String::write( AstWriter &aw ) const {
    TODO;
}

} // namespace PreAst
END_TL_NAMESPACE

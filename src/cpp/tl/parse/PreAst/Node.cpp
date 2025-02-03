#include "Node.h"

BEG_TL_NAMESPACE
namespace PreAst {

Node::Node( TlToken *token ) : token( token ) {
}

Node::~Node() {    
}

} // namespace PreAst
END_TL_NAMESPACE

#include "Node.h"

BEG_TL_NAMESPACE
namespace Pst {

Node::Node( Tok::Node *token ) : token( token ) {
}

Node::~Node() {    
}

} // namespace Pst
END_TL_NAMESPACE

#include "PNode.h"

BEG_TL_NAMESPACE
namespace Pst {

PNode::PNode( Tok::TNode *token ) : token( token ) {
}

PNode::~PNode() {    
}

} // namespace Pst
END_TL_NAMESPACE

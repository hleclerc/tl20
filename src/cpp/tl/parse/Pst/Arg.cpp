#include "Arg.h"

BEG_TL_NAMESPACE
namespace Pst {

Arg::Arg( Tok::Node *token, Str name, Node *node ) : token( token ), name( name ), node( node ) {
}

void Arg::display( Displayer &ds ) const {
    DS_OBJECT( Arg, name, node );
}

void Arg::write( Ast::Writer &aw ) const {
    TODO;
}

} // namespace Pst
END_TL_NAMESPACE

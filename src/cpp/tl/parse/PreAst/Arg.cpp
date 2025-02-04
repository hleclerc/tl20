#include "Arg.h"

BEG_TL_NAMESPACE
namespace PreAst {

Arg::Arg( TlToken *token, Str name, Node *node ) : token( token ), name( name ), node( node ) {
}

void Arg::display( Displayer &ds ) const {
    DS_OBJECT( Arg, name, node );
}

void Arg::write( AstWriter &aw ) const {
    TODO;
}

} // namespace PreAst
END_TL_NAMESPACE

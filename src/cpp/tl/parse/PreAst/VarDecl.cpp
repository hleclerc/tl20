#include "VarDecl.h"

BEG_TL_NAMESPACE
namespace PreAst {

VarDecl::VarDecl( Scope *parent_scope ) : scope( parent_scope ) {
}

void VarDecl::display( Displayer &ds ) const {
    DS_OBJECT( VarDecl, pos_in_parent_scope, is_a_func );
}

void VarDecl::write( AstWriter &aw ) const {
    TODO;
}

} // namespace PreAst
END_TL_NAMESPACE

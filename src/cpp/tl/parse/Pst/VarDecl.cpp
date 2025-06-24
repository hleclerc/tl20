#include "VarDecl.h"

BEG_TL_NAMESPACE
namespace Pst {

VarDecl::VarDecl( Scope *parent_scope ) : scope( parent_scope ) {
}

void VarDecl::display( Displayer &ds ) const {
    DS_OBJECT( VarDecl, pos_in_parent_scope, is_a_func );
}

void VarDecl::write( Ast &ast ) const {
    TODO;
}

} // namespace Pst
END_TL_NAMESPACE

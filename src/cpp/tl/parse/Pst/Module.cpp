#include "Module.h"

BEG_TL_NAMESPACE
namespace Pst {

void Module::display( Displayer &ds ) const {
    DS_OBJECT( Module, global_variables, block );
}

void Module::write( Ast::Writer &aw ) const {
    TODO;
}

} // namespace Pst
END_TL_NAMESPACE

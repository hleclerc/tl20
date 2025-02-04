#include "Module.h"

BEG_TL_NAMESPACE
namespace PreAst {

void Module::display( Displayer &ds ) const {
    DS_OBJECT( Module, global_variables, block );
}

void Module::write( AstWriter &aw ) const {
    TODO;
}

} // namespace PreAst
END_TL_NAMESPACE

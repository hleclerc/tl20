#include "Module.h"

BEG_TL_NAMESPACE
namespace PreAst {

Module::Module( TlToken *token ) : Block( token, nullptr, Scope::Type::Immediate ) {
}

void Module::display( Displayer &ds ) const {
    DS_OBJECT( Module, global_variables, scope, nodes );
}

void Module::write( AstWriter &aw ) const {
    TODO;
}

} // namespace PreAst
END_TL_NAMESPACE

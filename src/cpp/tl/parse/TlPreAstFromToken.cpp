#include "PreAst/Node_Variable.h"
#include "PreAst/FuncInfo.h"

#include "TlPreAstFromToken.h"
#include "TlToken.h"

#include "../support/P.h"

BEG_TL_NAMESPACE
using namespace PreAst;

TlPreAstFromToken::TlPreAstFromToken( Log &log, BumpPointerPool &pool ) : pool( pool ), log( log ) {
    module = pool.create<Module>( nullptr );
    module->scope.func_info = base_func_info();

    current_scope = &module->scope;
}

void TlPreAstFromToken::display( Displayer &ds ) const {
    ds << module;
}

void TlPreAstFromToken::write( AstWriter &aw ) const {

}

void TlPreAstFromToken::push( TlToken *token ) {
    module->nodes << make_node( token );
}

Node *TlPreAstFromToken::make_node( TlToken *token ) {
    switch ( token->type ) {
        case TlToken::Type::ParenthesisCall:
            P( token );
            TODO;
        case TlToken::Type::BracketCall:
            P( token );
            TODO;
        case TlToken::Type::BraceCall:
            P( token );
            TODO;
        case TlToken::Type::Variable:
            return pool.create<Node_Variable>( token, current_scope, token->content );
        case TlToken::Type::String:
            P( token );
            TODO;
    }
    return nullptr;
}

END_TL_NAMESPACE

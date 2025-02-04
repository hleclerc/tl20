#include "PreAst/Node_Variable.h"
#include "PreAst/Node_String.h"
#include "PreAst/Node_Call.h"
#include "PreAst/FuncInfo.h"

#include "TlPreAstFromToken.h"
#include "TlToken.h"

#include "../support/P.h"

BEG_TL_NAMESPACE
using namespace PreAst;

TlPreAstFromToken::TlPreAstFromToken( Log &log, BumpPointerPool &pool ) : pool( pool ), log( log ) {
    func_info = base_func_info();

    module = pool.create<Module>( nullptr );
    module->scope.func_info = base_func_info();
}

void TlPreAstFromToken::display( Displayer &ds ) const {
    ds << module;
}

void TlPreAstFromToken::write( AstWriter &aw ) const {

}

void TlPreAstFromToken::push( TlToken *token ) {
    module->nodes << make_node( &module->scope, token );
}

Node *TlPreAstFromToken::make_variable( PreAst::Scope *scope, TlToken *token ) {
    Node_Variable *res = pool.create<Node_Variable>( token, scope, token->content );
    variables << res;
    return res;
}

Node *TlPreAstFromToken::make_string( PreAst::Scope *scope, TlToken *token ) {
    Node_String *res = pool.create<Node_String>( token, token->content );
    return res;
}

Node *TlPreAstFromToken::make_call( PreAst::Scope *scope, TlToken *token ) {
    // result object
    Node_Call *call = pool.create<Node_Call>( token, scope );

    // func
    TlToken *func = token->first_child;
    call->func = make_node( &call->scope, func );

    // special args ?
    TlToken *arg = func->next;
    if ( auto *v = dynamic_cast<Node_Variable *>( call->func ) ) {
        auto iter = func_info->arg_types.find( v->name );
        if ( iter != func_info->arg_types.end() ) {
            for( FuncInfo::Behavior at : iter->second ) {
                switch ( at ) {
                    case FuncInfo::ArgType::ImmediateBlock: TODO;
                    case FuncInfo::ArgType::DelayedBlock: TODO;
                    case FuncInfo::ArgType::VarDecl: TODO;
                    case FuncInfo::ArgType::ArgDecl: TODO;
                    case FuncInfo::ArgType::InDecl: TODO;
                    case FuncInfo::ArgType::Value: TODO;
                }
            }

            // if ( iter->second.delayed_scope )
            //     call->scope.delayed = true;
            // for( const auto &f : iter->second.args )
            //     call->args << ApArg( arg, {}, f( this, &call->scope, arg, tok, current_qualifiers ) );
        }
    }

    // standard args
    for( ; arg; arg = arg->next )
        call->args << make_arg( &call->scope, arg );

    return call;

}

Node *TlPreAstFromToken::make_node( PreAst::Scope *scope, TlToken *token ) {
    switch ( token->type ) {
        case TlToken::Type::ParenthesisCall:
            return make_call( scope, token );
        case TlToken::Type::BracketCall:
            P( token );
            TODO;
        case TlToken::Type::BraceCall:
            P( token );
            TODO;
        case TlToken::Type::Variable:
            return make_variable( scope, token );
        case TlToken::Type::String:
            return make_string( scope, token );
    }
    return nullptr;
}

Arg TlPreAstFromToken::make_arg( PreAst::Scope *scope, TlToken *token ) {
    Arg res( token );

    // named arg ?
    // if ( Token::is_func( arg, "operator :", 2 ) ) {
    //     Token *name = arg->first_child->next;
    //     if ( name->type != Token::Type::Variable )
    //         error( "argument names must be variables", name );
    //     call->args.push_back( ast_pool.create<AstArg>( arg->src_filename, arg->src_content, arg->englo_str, arg->orig_str, name->str, make_node( &call->scope, name->next ) ) );
    //     continue;
    // }

    // unnamed arg
    res.node = make_node( scope, token );
    return res;

}

END_TL_NAMESPACE

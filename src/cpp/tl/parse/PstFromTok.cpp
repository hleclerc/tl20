#include "Pst/Node_VarDeclAndBlock.h"
#include "Pst/Node_Variable.h"
#include "Pst/Node_Module.h"
#include "Pst/Node_String.h"
#include "Pst/Node_Call.h"
#include "Pst/FuncType.h"

#include "PstFromTok.h"

#include "../support/P.h"

BEG_TL_NAMESPACE
using namespace Pst;

PstFromTok::PstFromTok( Log &log ) : log( log ) {
    base_scope.func_map = base_func_map();
    root_node = nullptr;
}

void PstFromTok::display( Displayer &dp ) const {
    dp << root_node;
}

PNode *PstFromTok::root() const {
    return root_node;
}

void PstFromTok::parse( Tok::TNode *root ) {
    root_node = make_node( &base_scope, root );
}

PNode *PstFromTok::make_node_variable( Scope *scope, Tok::TNode *token ) {
    Node_Variable *res = pool.create<Node_Variable>( token, scope, token->variable_ref );
    variable_refs << res;
    return res;
}

PNode *PstFromTok::make_node_module( Scope *scope, Tok::TNode *token ) {
    Scope *new_scope = pool.create<Scope>( scope );


    Node_Module *res = pool.create<Node_Module>( token );
    res->block = make_block( new_scope, token, &res->global_variables );
    return res;
}

PNode *PstFromTok::make_node_string( Scope *scope, Tok::TNode *token ) {
    Node_String *res = pool.create<Node_String>( token, token->string_content );
    return res;
}

PNode *PstFromTok::make_node_call( Scope *scope, Tok::TNode *token ) {
    // result object
    Node_Call *call = pool.create<Node_Call>( token, scope );

    // func
    Tok::TNode *func = token->first_child;
    call->func = make_node( &call->scope, func );

    // special args ?
    Tok::TNode *arg = func->next;
    if ( auto *v = dynamic_cast<Node_Variable *>( call->func ) ) {
        auto iter = call->scope.func_map->find( v->name );
        if ( iter != call->scope.func_map->end() ) {
            switch ( iter->second ) {
                case FuncType::ImmediateBlock: {
                    TODO;
                }
                
                case FuncType::Assignation: {
                    Node_VarDeclAndBlock *n = pool.create<Node_VarDeclAndBlock>( arg );
                    n->var_decl = make_var_decl( &call->scope, arg, false );
                    n->block = make_block( &n->var_decl->scope, arg->next );
                    call->args << Arg{ arg, {}, n };
                    return call;
                }
                
                case FuncType::Lambda: {
                    TODO;
                }

                case FuncType::Module: {
                    Node_Module *n = pool.create<Node_Module>( arg );
                    n->block = make_block( &call->scope, arg, &n->global_variables );
                    call->args << Arg{ arg, {}, n };
                    return call;
                }

                case FuncType::Class: {
                    TODO;
                }

                case FuncType::Test: {
                    TODO;
                }

                case FuncType::Def: {
                    TODO;
                }

                case FuncType::For: {
                    TODO;
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

PNode *PstFromTok::make_node( Scope *scope, Tok::TNode *token ) {
    switch ( token->type ) {
        case Tok::TNode::Type::ParenthesisCall:
            return make_node_call( scope, token );
        case Tok::TNode::Type::BracketCall:
            P( token );
            TODO;
        case Tok::TNode::Type::BraceCall:
            P( token );
            TODO;
        case Tok::TNode::Type::Variable:
            return make_node_variable( scope, token );
        case Tok::TNode::Type::String:
            return make_node_string( scope, token );
    }
    return nullptr;
}

Arg PstFromTok::make_arg( Scope *scope, Tok::TNode *token ) {
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

VarDecl *PstFromTok::make_var_decl( Scope *scope, Tok::TNode *token, bool func_by_default ) {
    VarDecl *res = pool.create<VarDecl>( scope );
    res->is_a_func = func_by_default;
    res->token = token;

    Tok::TNode *var = token;
    if ( var->type == Tok::TNode::Type::Variable ) {
        res->pos_in_parent_scope = scope->variable_names.push_back_ind( var->variable_ref );
    } else {
        TODO;
    }

    return res;
}

Block *PstFromTok::make_block( Scope *scope, Tok::TNode *token, Vec<GString> *global_variables ) {
    Block *res = pool.create<Block>( scope );
    res->scope.global_variables = global_variables;

    for( ; token; token = token->next )
        res->nodes << make_node( &res->scope, token );

    return res;

}

END_TL_NAMESPACE

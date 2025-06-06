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

void PstFromTok::display( Displayer &ds ) const {
    ds << root_node;
}

Node *PstFromTok::root() const {
    return root_node;
}

void PstFromTok::parse( Tok::Node *root ) {
    root_node = make_node( &base_scope, root );
}

Node *PstFromTok::make_node_variable( Pst::Scope *scope, Tok::Node *token ) {
    Node_Variable *res = pool.create<Node_Variable>( token, scope, token->content );
    variable_refs << res;
    return res;
}

Node *PstFromTok::make_node_string( Pst::Scope *scope, Tok::Node *token ) {
    Node_String *res = pool.create<Node_String>( token, token->content );
    return res;
}

Node *PstFromTok::make_node_call( Pst::Scope *scope, Tok::Node *token ) {
    // result object
    Node_Call *call = pool.create<Node_Call>( token, scope );

    // func
    Tok::Node *func = token->first_child;
    call->func = make_node( &call->scope, func );

    // special args ?
    Tok::Node *arg = func->next;
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

Node *PstFromTok::make_node( Pst::Scope *scope, Tok::Node *token ) {
    switch ( token->type ) {
        case Tok::Node::Type::ParenthesisCall:
            return make_node_call( scope, token );
        case Tok::Node::Type::BracketCall:
            P( token );
            TODO;
        case Tok::Node::Type::BraceCall:
            P( token );
            TODO;
        case Tok::Node::Type::Variable:
            return make_node_variable( scope, token );
        case Tok::Node::Type::String:
            return make_node_string( scope, token );
    }
    return nullptr;
}

Arg PstFromTok::make_arg( Pst::Scope *scope, Tok::Node *token ) {
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

VarDecl *PstFromTok::make_var_decl( Pst::Scope *scope, Tok::Node *token, bool func_by_default ) {
    VarDecl *res = pool.create<VarDecl>( scope );
    res->is_a_func = func_by_default;
    res->token = token;

    Tok::Node *var = token;
    if ( var->type == Tok::Node::Type::Variable ) {
        res->pos_in_parent_scope = scope->variable_names.push_back_ind( var->content );
    } else {
        TODO;
    }

    return res;
}

Block *PstFromTok::make_block( Pst::Scope *scope, Tok::Node *token, Vec<Str> *global_variables ) {
    Block *res = pool.create<Block>( scope );
    res->scope.global_variables = global_variables;

    for( ; token; token = token->next )
        res->nodes << make_node( &res->scope, token );

    return res;

}

END_TL_NAMESPACE

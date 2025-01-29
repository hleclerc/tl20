#include "TlToken.h"

BEG_TL_NAMESPACE

void TlToken::repl_in_graph_by( TlToken *token ) {
    // update_englo_str_rec( token );
    
    if ( parent ) {
        if ( parent->first_child == this )
            parent->first_child = token;
        if ( parent->last_child == this )
            parent->last_child = token;
    }

    if ( prev )
        prev->next = token;
    if ( next )
        next->prev = token;

    token->parent = parent;
    token->prev = prev;
    token->next = next;

    parent = nullptr;
    prev = nullptr;
    next = nullptr;
}

void TlToken::add_child( TlToken *child ) {
    if ( last_child )
        last_child->next = child;
    else
        first_child = child;
    child->parent = this;
    child->prev = last_child;
    last_child = child;
}

void TlToken::display( Displayer &ds ) const {
    Str repr;
    switch ( type ) {
        case Type::Root: repr = "root"; break;
        case Type::Variable: repr = "var"; break;
        case Type::String: repr = "str"; break;
        case Type::ParenthesisCall: repr = "call"; break;
        case Type::BraceCall: repr = "call_brace"; break;
        case Type::BracketCall: repr = "call_bracket"; break;
    }

    if ( content.size() )
        repr += "{" + Str( content ) + "}";

    if ( first_child ) {
        ds.set_next_head( repr );
        ds.start_array();
        for( TlToken *child = first_child; child; child = child->next )
            ds << *child;
        ds.end_array();
    } else
        ds << repr;
}


END_TL_NAMESPACE

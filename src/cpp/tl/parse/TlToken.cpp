#include "../support/ASSERT.h"
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

Str TlToken::condensed() const {
    auto disp_call = [&]( const Str &opening, const Str &closing ) {
        // Str res = ASSERTED( first_child )->condensed();
        Str res = opening;
        for( TlToken *child = first_child; child; child = child->next ) {
            res += ( child == first_child ? "" : "," );
            res += child->condensed();
        }
        return res + closing;
    };

    auto escaped_string = [&]( const Str &content ) {
        Str res;
        for( char c : content ) {
            if ( c == '\n' ) { res += "\\n"; continue; }
            if ( c == '\t' ) { res += "\\t"; continue; }
            res += c;
        }
        return res;
    };

    switch ( type ) {
        case Type::ParenthesisCall: return disp_call( "(", ")" );
        case Type::BracketCall: return disp_call( "[", "]" );
        case Type::BraceCall: return disp_call( "{", "}" );
        case Type::Variable: return content;
        case Type::String: return "\"" + escaped_string( content ) + "\"";
    }
}

void TlToken::display( Displayer &ds ) const {
    ds << condensed();
}


END_TL_NAMESPACE

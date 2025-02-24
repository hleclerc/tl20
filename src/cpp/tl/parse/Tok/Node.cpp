#include "../../support/ASSERT.h"
#include "Node.h"

BEG_TL_NAMESPACE
namespace Tok {

void Node::repl_in_graph_by( Node *token ) {
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

void Node::add_child( Node *child ) {
    if ( last_child )
        last_child->next = child;
    else
        first_child = child;
    child->parent = this;
    child->prev = last_child;
    last_child = child;
}

Str Node::condensed() const {
    auto disp_children = [&]() {
        Str res;
        for( Node *child = first_child; child; child = child->next ) {
            ASSERT( child != this ); // test self ref
                            
            res += ( child == first_child ? "" : "," );
            res += child->condensed();
        }
        return res;
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
        case Type::ParenthesisCall: return "(" + disp_children() + ")";
        case Type::BracketCall: return "[" + disp_children() + "]";
        case Type::BraceCall: return "{" + disp_children() + "}";
        case Type::Variable: return content;
        case Type::String: return "\"" + escaped_string( content ) + "\"";
        case Type::Root: return disp_children();
    }
}

void Node::display( Displayer &ds ) const {
    ds << condensed();
}

} // namespace Tok
END_TL_NAMESPACE

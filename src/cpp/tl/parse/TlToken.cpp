#include "TlToken.h"

BEG_TL_NAMESPACE

void TlToken::display( Displayer &ds ) const {
    Str repr;
    switch ( type ) {
        case Type::Root: repr = "R"; break;
        case Type::Variable: repr = "V"; break;
        case Type::Number: repr = "N"; break;
        case Type::ParenthesisCall: repr = "P"; break;
    }

    if ( content.size() )
        repr += "{" + content + "}";

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

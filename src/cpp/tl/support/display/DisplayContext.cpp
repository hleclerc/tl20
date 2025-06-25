#include "DisplayContext.h"

BEG_TL_NAMESPACE

//     os << "\n";

void DisplayContext::new_item( std::ostream &os ) {
    if ( ! std::exchange( first_item, false ) )
        os << "\n";
    os << beg_line;
}

void DisplayContext::incr() {
     beg_line += "  ";
}

void DisplayContext::decr() { 
    beg_line.resize( beg_line.size() - 2 );
}

END_TL_NAMESPACE

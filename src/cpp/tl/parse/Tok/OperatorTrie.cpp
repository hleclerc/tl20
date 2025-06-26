#include "OperatorTrie.h"

BEG_TL_NAMESPACE
namespace Tok {

class OperatorTrie_Tl : public OperatorTrie {
public:
    OperatorTrie_Tl() {
        const int I = std::numeric_limits<int>::max();
        int curr_prio = 0;

        auto decl = [&]( Str n, int take_left, bool l2r, int rch ) {
            auto *od = new OperatorData{ 
                .str = n, 
                .name = "operator " + n,
                .priority = curr_prio, 
                .take_left = take_left, 
                .l2r = l2r, 
                .min_rch = rch == I ? 0 : rch,
                .max_rch = rch,
            };
            if ( ( n[ 0 ] >= 'a' &&  n[ 0 ] <= 'z' ) || ( n[ 0 ] >= 'A' &&  n[ 0 ] <= 'Z' ) || n[ 0 ] == '_' )
                register_letter_operator( od );
            else
                register_symbol_operator( od );
        };

        auto std_sop = [&]( Str n, int take_left, bool l2r, int rch ) {
        };
        
        const int M = 2; // Mandatory
        const int O = 1; // Optional

        prio_assign = ++curr_prio;
        decl( ":="        , /*take_left*/ M, /*l2r*/ 0, /*rch*/ 1 );
        decl( "::"        , /*take_left*/ M, /*l2r*/ 0, /*rch*/ 1 );

        prio_lambda = ++curr_prio;
        decl( "=>"        , /*take_left*/ M, /*l2r*/ 1, /*rch*/ I );

        prio_argpar = ++curr_prio;
        decl( "pertinence", /*take_left*/ M, /*l2r*/ 1, /*rch*/ 1 );
        decl( "when"      , /*take_left*/ M, /*l2r*/ 0, /*rch*/ 1 );

        prio_argval = ++curr_prio;
        decl( "="         , /*take_left*/ M, /*l2r*/ 0, /*rch*/ 1 );
        decl( ":"         , /*take_left*/ M, /*l2r*/ 0, /*rch*/ 1 );

        prio_argval = ++curr_prio;
        decl( "=="        , /*take_left*/ M, /*l2r*/ 1, /*rch*/ 1 );
        decl( "<="        , /*take_left*/ M, /*l2r*/ 1, /*rch*/ 1 );
        decl( ">="        , /*take_left*/ M, /*l2r*/ 1, /*rch*/ 1 );
        decl( "!="        , /*take_left*/ M, /*l2r*/ 1, /*rch*/ 1 );
        decl( "<"         , /*take_left*/ M, /*l2r*/ 1, /*rch*/ 1 );
        decl( ">"         , /*take_left*/ M, /*l2r*/ 1, /*rch*/ 1 );

        prio_and = ++curr_prio;
        decl( "and"       , /*take_left*/ M, /*l2r*/ 1, /*rch*/ 1 );
        decl( "or"        , /*take_left*/ M, /*l2r*/ 1, /*rch*/ 1 );

        prio_add = ++curr_prio;
        decl( "++"        , /*take_left*/ M, /*l2r*/ 1, /*rch*/ 0 );
        decl( "+"         , /*take_left*/ O, /*l2r*/ 1, /*rch*/ 1 );
        decl( "-"         , /*take_left*/ O, /*l2r*/ 1, /*rch*/ 1 );

        prio_mul = ++curr_prio;
        decl( "*"         , /*take_left*/ M, /*l2r*/ 1, /*rch*/ 1 );
        decl( "/"         , /*take_left*/ M, /*l2r*/ 1, /*rch*/ 1 );

        prio_pow = ++curr_prio;
        decl( "**"        , /*take_left*/ M, /*l2r*/ 1, /*rch*/ 1 );

        prio_call = ++curr_prio;

        prio_dot = ++curr_prio;
        decl( "."         , /*take_left*/ M, /*l2r*/ 1, /*rch*/ 1 );
        decl( "->"        , /*take_left*/ M, /*l2r*/ 1, /*rch*/ 1 );
    }
};

OperatorTrie *base_operator_trie() {
    static OperatorTrie_Tl res;
    return &res;
}

OperatorTrie::OperatorTrie() : symbol_map( nullptr ) {
}

OperatorTrie::~OperatorTrie() {
    for( auto &p : letter_map )
        delete p.second;
    delete symbol_map;
}

OperatorTrie::CharMap::~CharMap() {
    for( PI i = 0; i < 256; ++i )
        delete next[ i ];
    delete operator_data;
} 

void OperatorTrie::OperatorData::display( Displayer &dp ) const {
    dp << name;
}

void OperatorTrie::register_letter_operator( OperatorData *operator_data ) {
    letter_map[ operator_data->str ] = operator_data;
}

void OperatorTrie::register_symbol_operator( OperatorData *operator_data ) {
    StrView str = operator_data->str;
    CharMap *prev = nullptr;
    CharMap **p = &symbol_map;
    while ( true ) {
        if ( ! *p ) {
            *p = new CharMap;
            (*p)->operator_data = nullptr;
            (*p)->prev = prev;
            for( PI i = 0; i < 256; ++i )
                (*p)->next[ i ] = nullptr;
        }

        if ( str.empty() )
            break;
        p = &(*p)->next[ str[ 0 ] ];
        str.remove_prefix( 1 );
    }

    (*p)->operator_data = operator_data;
}

OperatorTrie::OperatorData *OperatorTrie::symbol_op( StrView str ) {
    // go as far as we can
    OperatorData *o = nullptr;
    PI len = 0;
    for( CharMap *p = symbol_map; p; p = p->next[ str[ len++ ] ] ) {
        if ( p->operator_data )
            o = p->operator_data;
        if ( len == str.size() )
            break;
    }

    return o;
}

OperatorTrie::OperatorData *OperatorTrie::letter_op( StrView str ) {
    auto iter = letter_map.find( Str( str ) );
    if ( iter != letter_map.end() )
        return iter->second;
    return nullptr;
}

} // namespace Tok
END_TL_NAMESPACE

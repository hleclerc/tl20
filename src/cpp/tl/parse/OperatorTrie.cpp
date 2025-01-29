#include "OperatorTrie.h"

BEG_TL_NAMESPACE

OperatorTrie *OperatorTrie::default_tl_operator_trie() {
    static OperatorTrie res;
    return &res;
}

OperatorTrie::OperatorTrie() : symbol_map( nullptr ) {
    init_default_operators();
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

void OperatorTrie::OperatorData::display( Displayer &ds ) const {
    ds << name;
}

void OperatorTrie::init_default_operators() {
    int curr_prio = 0;

    ++curr_prio;
    register_symbol_operator( new OperatorData{ .str = "++" , .name = "operator ++" , .priority = curr_prio, .take_left = 0, .l2r = true, .max_rch = 0 } );
    register_symbol_operator( new OperatorData{ .str = "+"  , .name = "operator +"  , .priority = curr_prio, .take_left = 1, .l2r = true, .max_rch = 1 } );

    ++curr_prio;
    register_symbol_operator( new OperatorData{ .str = "*"  , .name = "operator *"  , .priority = curr_prio, .take_left = 2, .l2r = true, .max_rch = 1 } );

    ++curr_prio;
    register_letter_operator( new OperatorData{ .str = "and", .name = "operator and", .priority = curr_prio, .take_left = 2, .l2r = true, .max_rch = 1 } );
    register_letter_operator( new OperatorData{ .str = "or" , .name = "operator or" , .priority = curr_prio, .take_left = 2, .l2r = true, .max_rch = 1 } );
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

END_TL_NAMESPACE

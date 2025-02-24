from interval import Interval

class Node:
    def __init__( self, name ):
        self.call_func = False
        self.name = name

        self.next = [] # list of tuple ( first, last, node, frequency )

    def add_next( self, node, first, last = None ):
        if last is None:
            last = first
        self.next.append( ( ord( first ), ord( last ), node, 0 ) )

    def next_code( self, beg_code, end_code, prefix ):
        txt = ""
        for first, last, nxt, freq in self.next:
            txt += prefix + f'if ( '

            if first == last:
                txt += f'c == { first }'
            else:
                txt += f'c >= { first } && c <= { last }'

            txt += f' ) goto beg_{ nxt.name };\n'
        return txt
    

class Writer:
    def __init__( self ):
        self.visited = []
        self.txt = ""

    def write( self, node ):
        self.visited.append( node )

        self.txt += f'\n'
        self.txt += f"    beg_{ node.name }:\n"
        self.txt += f"        if ( cur == end ) {{ restart_jump = &&beg_{ node.name }; return; }}\n"
        self.txt += f"        c = *( cur++ );\n"

        # 
        if len( node.next ) == 0:
            self.txt += f'        goto beg_root;\n'
            return

        # 1 char or goto
        self.txt += f'        if ( c < 0x80 ) {{\n'
        self.txt += node.next_code( 0x00, 0x80, '            ' )
        self.txt += f'        }}\n'
        self.txt += f'        if ( c < 0xC0 ) goto err_unexpected_continuation;\n'
        self.txt += f'        if ( c < 0xE0 ) curr_codepoint = c & ( 0xFF >> 3 ); goto cn21_{ node.name }; // 2 chars\n'
        self.txt += f'        if ( c < 0xF0 ) curr_codepoint = c & ( 0xFF >> 3 ); goto cn31_{ node.name }; // 3 chars\n'
        self.txt += f'        if ( c < 0xF8 ) curr_codepoint = c & ( 0xFF >> 4 ); goto cn41_{ node.name }; // 4 chars\n'
        self.txt += f'        if ( c < 0xFC ) curr_codepoint = c & ( 0xFF >> 5 ); goto cn51_{ node.name }; // 5 chars\n'
        self.txt += f'        if ( c < 0xFE ) curr_codepoint = c & ( 0xFF >> 5 ); goto cn61_{ node.name }; // 6 chars\n'
        self.txt += f'        if ( c < 0xFF ) goto err_unassigned_char;\n'
        self.txt += f'        goto after_eof\n'

        # continuation
        nb_bits = [ 7 ] + [ 7 - n + 6 * ( n - 1 ) for n in range( 2, 7 ) ]
        for n in [ 6, 5, 4, 3, 2 ]:
            self.txt += f'\n'
            for i in range( 1, n ):
                self.txt += f'    cn{ n }{ i }_{ node.name }: // { n } chars\n'
                self.txt += f'        if ( cur == end ) {{ restart_jump = &&cn{ n }{ i }_{ node.name }; return; }}\n'
                self.txt += f'        c = *( cur++ );\n'
                self.txt += f'        if ( ( c & 0b11000000 ) != 0b10000000 )\n'
                self.txt += f'            goto _err_expecting_continuation;\n'
                self.txt += f'        curr_codepoint = ( curr_codepoint << 6 ) | ( c & 0b00111111 );\n'

            beg_code = 2 ** nb_bits[ n - 2 ]
            end_code = 2 ** nb_bits[ n - 1 ]
            self.txt += node.next_code( beg_code, end_code, '        ' )

        # recursive write
        for first, last, nxt, freq in node.next:
            if nxt not in self.visited:
                self.write( nxt )


def insert( filename, txt_to_insert, beg_txt = "// beg generated code", end_txt = "// end generated code" ):
    with open( filename, "r" ) as finp:
        txt = finp.read()
    beg = txt.find( beg_txt )
    end = txt.find( end_txt )

    with open( filename, "w" ) as fout:
        fout.write( txt[ : beg + len( beg_txt ) ] + "\n" )
        fout.write( txt_to_insert )
        fout.write( txt[ end : ] )

# main node entries =========================================
variable = Node( "variable" )
operator = Node( "operator" )
comment  = Node( "comment" )
number   = Node( "number" )
string   = Node( "string" )
root     = Node( "root" )

# root ======================================================
root.add_next( variable, 'a', 'z' )
root.add_next( variable, 'A', 'Z' )
root.add_next( variable, '_' )

root.add_next( comment, '#' )

root.add_next( number, '0', '9' )

root.add_next( string, '"' )

# variable ======================================================
variable.add_next( variable, 'a', 'z' )
variable.add_next( variable, 'A', 'Z' )
variable.add_next( variable, '_' )
variable.call_func = True

# number ======================================================
number.add_next( number, '0', '9' )
number.add_next( number, '.' )
number.call_func = True


# ================================================================
wr = Writer()
wr.write( root )
print( wr.txt )
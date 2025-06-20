from IntervalSet import IntervalSet
import os

# all the nodes
all_the_nodes = []

class Node:
    """
        `exit_code` -> code to be executed when it "escapes"
    """
    def __init__( self, name, defined_in_cpp = False, exit_code = "" ):
        self.defined_in_cpp = defined_in_cpp
        self.exit_code = exit_code
        self.name = name

        self.next = [] # list of Next

        all_the_nodes.append( self )

    def add_next( self, node, first, last = None, code = "", exit = False ):
        if last is None:
            last = first

        if isinstance( first, str ):
            first = ord( first )
        if isinstance( last, str ):
            last = ord( last )

        rem = IntervalSet( first, last + 1 )
        for next in self.next:
            rem -= IntervalSet( next.beg, next.end )

        for beg, end in rem.intervals:
            self.next.append( Next( beg, end, node, code, exit ) )
    
    def add_exit( self, node, first, last = None, code = "" ):
        self.add_next( node, first, last, code, True )

    def fill_rem( self, node ):
        rem = IntervalSet( 0, 2**31 )
        for next in self.next:
            rem -= IntervalSet( next.beg, next.end )

        for beg, end in rem.intervals:
            self.next.append( Next( beg, end, node, 0, exit = True ) )

    def next_code( self, beg_code, end_code, prefix ):
        next_list = []
        for next in self.next:
            if next.beg < end_code and next.end > beg_code:
                next_list.append( next )
        return self._next_code( next_list, prefix )

    def _next_code( self, next_list, prefix ):
        assert len( next_list )
        if len( next_list ) == 1:
            next = next_list[ 0 ]
            res = ""
            if next.code:
                res += prefix + next.code + "\n"
            if next.exit and self.exit_code:
                res += prefix + self.exit_code + "\n"
            res += prefix + f'goto on_{ next.node.name };\n'
            return res

        begs = []
        for next in next_list:
            begs.append( next.beg )
        begs.sort()
        begs.pop( 0 )

        sep = begs[ int( len( begs ) / 2 ) ]
        
        # TODO: optimize begs selection
        txt = ""
        txt += prefix + f'if ( c >= { sep } ) {{\n'
        txt += self._next_code( list( filter( lambda n: n.beg >= sep, next_list ) ), prefix + '    ' )
        txt += prefix + f'}} else {{\n'
        txt += self._next_code( list( filter( lambda n: n.beg < sep, next_list ) ), prefix + '    ' )
        txt += prefix + f'}}\n'
        return txt

class Next:
    def __init__( self, beg, end, node, code = "", exit = False, freq = 1 ):
        self.beg = beg 
        self.end = end 
        self.node = node 
        self.code = code 
        self.exit = exit 
        self.freq = freq


class Writer:
    def __init__( self ):
        self.txt = ""

    def write( self, node ):
        self.txt += f'\n'
        self.txt += f"    on_{ node.name }: {{\n"
        self.txt += f"        if ( cur == end ) {{ curr_tok_stored_content.append( StrView( curr_tok_ptr_content, end ) ); restart_jump = &&on_{ node.name }; return; }}\n"
        self.txt += f"        const unsigned char c = *( cur++ );\n"

        # 1 char or goto
        self.txt += f'        if ( c < 0x80 ) {{\n'
        self.txt += node.next_code( 0x00, 0x80, '            ' )
        self.txt += f'        }}\n'
        self.txt += f'        if ( c < 0xC0 ) goto err_unexpected_continuation;\n'
        self.txt += f'        if ( c < 0xE0 ) {{ curr_codepoint = c & ( 0xFF >> 3 ); goto cn21_{ node.name }; }}// 2 chars\n'
        self.txt += f'        if ( c < 0xF0 ) {{ curr_codepoint = c & ( 0xFF >> 3 ); goto cn31_{ node.name }; }}// 3 chars\n'
        self.txt += f'        if ( c < 0xF8 ) {{ curr_codepoint = c & ( 0xFF >> 4 ); goto cn41_{ node.name }; }}// 4 chars\n'
        self.txt += f'        if ( c < 0xFC ) {{ curr_codepoint = c & ( 0xFF >> 5 ); goto cn51_{ node.name }; }}// 5 chars\n'
        self.txt += f'        if ( c < 0xFE ) {{ curr_codepoint = c & ( 0xFF >> 6 ); goto cn61_{ node.name }; }}// 6 chars\n'
        if node.exit_code:
            self.txt += '        ' + node.exit_code + '\n'
        self.txt += f'        if ( c < 0xFF ) goto err_unassigned_char;\n'
        self.txt += f'        goto after_eof;\n'
        self.txt += f"    }}\n"

        # continuation bytes for utf8
        nb_bits = [ 7 ] + [ 7 - n + 6 * ( n - 1 ) for n in range( 2, 7 ) ]
        for n in range( 2, 7 ):
            self.txt += f'\n'
            for i in range( 1, n ):
                self.txt += f'    cn{ n }{ i }_{ node.name }: {{ // { n } chars\n'
                self.txt += f'        if ( cur == end ) {{ restart_jump = &&cn{ n }{ i }_{ node.name }; return; }}\n'
                self.txt += f'        const unsigned char c = *( cur++ );\n'
                self.txt += f'        if ( ( c & 0b11000000 ) != 0b10000000 )\n'
                self.txt += f'            goto err_expecting_continuation;\n'
                self.txt += f'        curr_codepoint = ( curr_codepoint << 6 ) | ( c & 0b00111111 );\n'
                self.txt += f'    }}\n'
       
            self.txt += f'    TODO;\n'

            # beg_code = 2 ** nb_bits[ n - 2 ]
            # end_code = 2 ** nb_bits[ n - 1 ]
            # self.txt += node.next_code( beg_code, end_code, '        ' )


def insert( filename, txt_to_insert, beg_txt = "// beg generated code", end_txt = "// end generated code" ):
    with open( filename, "r" ) as finp:
        txt = finp.read()
    beg = txt.find( beg_txt )
    end = txt.find( end_txt )

    with open( filename, "w" ) as fout:
        fout.write( txt[ : beg + len( beg_txt ) ] + "\n" )
        fout.write( txt_to_insert )
        fout.write( txt[ end : ] )

# helpers
letters = [ ( 'a', 'z' ), ( 'A', 'Z' ), ( '_', '_' ) ]

# main node entries =========================================
unauthorized_char = Node( "unauthorized_char", defined_in_cpp = True ) # not authorized character
eof = Node( "eof", defined_in_cpp = True ) # end of file

# variable ======================================================
variable = Node( "variable", exit_code = "_on_variable( cur - 1 );" )

for b, e in letters:
    variable.add_next( variable, b, e )
variable.add_next( variable, '0', '9' )

# number ======================================================
number = Node( "number", exit_code = "_on_number( cur - 1 );" )
numbec = Node( "numbec", exit_code = "_on_number( cur - 1 );" )
numbee = Node( "numbee", exit_code = "_on_number( cur - 1 );" ) 
numbep = Node( "numbep", exit_code = "_on_number( cur - 1 );" )

number.add_next( number, '0', '9' )
number.add_next( numbec, '.' )
number.add_next( numbee, 'e' )
number.add_next( numbee, 'E' )
for b, e in letters:
    number.add_next( numbep, b, e )

# after a first .
numbec.add_next( numbec, '0', '9' )
numbec.add_next( numbee, 'e' )
numbec.add_next( numbee, 'E' )
for b, e in letters:
    numbec.add_next( numbep, b, e )

# first char after an 'e' or a 'E'
for s in '+-':
    numbee.add_next( numbep, s, s )
numbee.add_next( numbep, '0', '9' )
for b, e in letters:
    numbee.add_next( numbep, b, e )

 # 2nd or more char after 'e' or a 'E'
numbep.add_next( numbep, '0', '9' )
for b, e in letters:
    numbep.add_next( numbep, b, e )



# new_line ======================================================
new_line = Node( "new_line", exit_code = "_on_new_line( cur - 1 );" )

new_line.add_next( new_line, '\r' )
new_line.add_next( new_line, '\t' )
new_line.add_next( new_line, ' ' )

# operator ======================================================
operator = Node( "operator", exit_code = "_on_operator( cur - 1 );" )

# punctuation ===================================================
semicolon = Node( "semicolon", exit_code = "_on_semicolon();" )
comma = Node( "comma", exit_code = "_on_comma();" )

# string ======================================================
string   = Node( "string", exit_code = "_on_string( cur - 1 );" )

# comment ======================================================
comment  = Node( "comment", exit_code = "_on_comment( cur - 1 );" )

# comment ======================================================
space  = Node( "space", exit_code = "_on_space( cur - 1 );" )

# ================================================================
# start machines
for node in all_the_nodes:
    if node.defined_in_cpp:
        continue

    node.add_exit( variable, 'a', 'z' )
    node.add_exit( variable, 'A', 'Z' )
    node.add_exit( variable, '_' )

    for o in "?.:/=+%*^-!(){}[]&@":
        node.add_exit( operator, o )

    node.add_exit( semicolon, ';' )
    node.add_exit( comma, ',' )

    node.add_exit( new_line, '\n' )

    node.add_exit( comment, '#' )

    node.add_exit( number, '0', '9' )

    node.add_exit( string, '"' )
    
    node.add_exit( space, ' ' )

    node.add_exit( eof, 255 )

    node.fill_rem( unauthorized_char )

# ================================================================
wr = Writer()
for node in all_the_nodes:
    if node.defined_in_cpp:
        continue
    wr.write( node )

with open( os.path.join( os.path.dirname( __file__ ), "TokFromTxt.gen" ), "w" ) as fout:
    fout.write( wr.txt )

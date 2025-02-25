class IntervalSet:
    def __init__( self, beg = 2**31, end = 2**31 ):
        if beg < end:
            self.intervals = [ ( beg, end ) ]
        else:
            self.intervals = []
        
    def __repr__( self ):
        return '[' + ','.join( str( i ) for i in self.intervals ) + ']'

    def __invert__( self ):
        res = IntervalSet()
        prev = 0
        for beg, end in self.intervals:
            if prev < beg:
                res.intervals.append( ( prev, beg ) )
            prev = end
        if prev < 2**31:
            res.intervals.append( ( prev, 2**31 ) )
        return res

    def __isub__( self, that ):
        self &= ~that
        return self

    def __iand__( self, that ):
        events = []
        for beg, end in self.intervals:
            events.append( [ beg, +1 ] )
            events.append( [ end, -1 ] )
        for beg, end in that.intervals:
            events.append( [ beg, +1 ] )
            events.append( [ end, -1 ] )

        events.sort()

        # agglomerate
        for n in range( len( events ) ):
            for m in range( n + 1, len( events ) ):
                if events[ n ] == events[ m ]:
                    events[ n ][ 1 ] += events[ m ][ 1 ]
                    events[ m ][ 1 ] = 0

        # filter
        events = filter( lambda x: x[ 1 ], events )

        # new intervals
        new_intervals = []
        beg = None
        n = 0
        for event in events:
            if n == 2 and event[ 1 ] < 0:
                new_intervals.append( ( beg, event[ 0 ] ) )

            n += event[ 1 ]

            if n == 2:
                beg = event[ 0 ]

        self.intervals = new_intervals

        return self

class Node:
    def __init__( self, name, has_next = True ):
        self.call_func = False
        self.has_next = has_next
        self.name = name

        self.next = [] # list of Next

    def add_next( self, node, first, last = None ):
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
            self.next.append( Next( beg, end, node ) )

    def fill_remaining( self, node ):
        rem = IntervalSet( 0, 2**31 )
        for next in self.next:
            rem -= IntervalSet( next.beg, next.end )

        for beg, end in rem.intervals:
            self.next.append( Next( beg, end, node, 0 ) )

    def next_code( self, beg_code, end_code, prefix ):
        next_list = []
        for next in self.next:
            if next.beg < end_code and next.end > beg_code:
                next_list.append( next )
        return self._next_code( next_list, prefix )

    def _next_code( self, next_list, prefix ):
        assert len( next_list )
        if len( next_list ) == 1:
            return prefix + f'goto beg_{ next_list[ 0 ].node.name };\n'

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
    def __init__( self, beg, end, node, freq = 1 ):
        self.beg = beg 
        self.end = end 
        self.node = node 
        self.freq = freq


class Writer:
    def __init__( self ):
        self.visited = []
        self.txt = ""

    def write_for_single_bytes( self, node ):
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
        self.txt += f'        if ( c < 0xFE ) curr_codepoint = c & ( 0xFF >> 6 ); goto cn61_{ node.name }; // 6 chars\n'
        self.txt += f'        if ( c < 0xFF ) goto err_unassigned_char;\n'
        self.txt += f'        goto after_eof\n'

        # recursive write
        for next in node.next:
            if next.node.has_next and next.node not in self.visited:
                self.write_for_single_bytes( next.node )

    def write_continuations( self ):
        for node in self.visited:
            nb_bits = [ 7 ] + [ 7 - n + 6 * ( n - 1 ) for n in range( 2, 7 ) ]
            for n in range( 2, 7 ):
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

unauthch = Node( "unauthch" )
unauthch.has_next = False

nr_nodes = [
    variable,
    operator,
    comment,
    number,
    string,
]

# root ======================================================
root.add_next( variable, 'a', 'z' )
root.add_next( variable, 'A', 'Z' )
root.add_next( variable, '_' )

root.add_next( comment, '#' )

root.add_next( number, '0', '9' )

root.add_next( string, '"' )

root.fill_remaining( unauthch )

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
for node in nr_nodes:
    for next in root.next:
        node.add_next( next.node, next.beg, next.end - 1 )

# ================================================================
wr = Writer()
wr.write_for_single_bytes( root )
wr.write_continuations()
print( wr.txt )

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

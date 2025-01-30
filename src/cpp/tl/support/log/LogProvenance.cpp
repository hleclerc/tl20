#include "LogProvenance.h"
#include <fstream>
#include <tuple>

LogProvenance::LogProvenance( StrView filename, StrView content, const std::array<ST,4> &offsets ) : filename( filename ), offsets( offsets ) {
    if ( offsets[ 0 ] <= offsets[ 1 ] )
        init_data_from_offsets( content, offsets );
}

LogProvenance::LogProvenance( StrView filename, const std::array<ST,4> &offsets ) : filename( filename ), offsets( offsets ) {
    if ( offsets[ 0 ] <= offsets[ 1 ] ) {
        // internal content ?
        if ( filename.find( "internal://" ) == 0 ) {
            this->filename = "(internal)";
            std::string content = std::string{ filename.substr( 11 ) };
            init_data_from_offsets( content.c_str(), offsets );
        } else {
            std::ifstream file( std::string{ filename } );
            if ( file ) {
                std::string content{
                    std::istreambuf_iterator<char>( file ),
                    std::istreambuf_iterator<char>(      )
                };
                init_data_from_offsets( content.c_str(), offsets );
            } else {
                this->offsets[ 0 ] = this->offsets[ 1 ] + 1;
            }
        }
    }
}

void LogProvenance::display_inline_extract( std::ostream &os, const StyleOfTextLog &style ) const {
    if ( complete_lines.empty() )
        return;

    // simple case: we don't have the offsets
    if ( offsets[ 0 ] > offsets[ 1 ] ) {
        os << complete_lines[ 0 ];
        return;
    }

    // need to cut on the left or on the right ?
    std::size_t max_line_size = complete_lines[ 0 ].size();
    std::size_t glo_beg_in_line = 0;
    std::size_t glo_end_in_line = max_line_size;
    if ( max_line_size > style.max_line_size ) {
        std::size_t mid = ( cols[ 1 ] + cols[ 2 ] ) / 2;
        // we have to cut the left part ?
        if ( mid > style.max_line_size / 2 ) {
            // we also have to cut the right part ?
            if ( mid - style.max_line_size / 2 + style.max_line_size < max_line_size ) {
                glo_beg_in_line = mid - style.max_line_size / 2;
                glo_end_in_line = mid - style.max_line_size / 2 + style.max_line_size;
            } else {
                glo_beg_in_line = max_line_size - style.max_line_size;
                glo_end_in_line = max_line_size;
            }
        } else {
            glo_beg_in_line = 0;
            glo_end_in_line = style.max_line_size;
        }
    }

    // else, we go through the lines
    bool cut_beg = glo_beg_in_line > 0;
    bool cut_end = glo_end_in_line < complete_lines[ 0 ].size();
    std::size_t beg_in_line = cut_beg ? glo_beg_in_line + style.underline_ellipsis.size() : 0;
    std::size_t end_in_line = cut_end ? glo_end_in_line - style.underline_ellipsis.size() : complete_lines[ 0 ].size();

    if ( cut_beg )
        os << style.underline_ellipsis;
    os << complete_lines[ 0 ].substr( beg_in_line, end_in_line - beg_in_line );
    if ( cut_end )
        os << style.underline_ellipsis;
}

void LogProvenance::display_text_header( std::ostream &os, const StyleOfTextLog &style ) const {
    os << filename << ":";
    if ( offsets[ 0 ] <= offsets[ 1 ] )
        os << lines[ 1 ] + style.offset_line << ":" << cols[ 1 ] + style.offset_col << ":";
    os << " ";
}

void LogProvenance::display_text_extract( std::ostream &os, const StyleOfTextLog &style, bool prefix_with_newline ) const {
    if ( prefix_with_newline && complete_lines.size() )
        os << "\n";

    // simple case: we don't have the offsets
    if ( offsets[ 0 ] > offsets[ 1 ] ) {
        for( std::size_t i = 0; i < complete_lines.size(); ++i )
            os << ( i ? "\n" : "" ) << style.indented_info_prefix << complete_lines[ i ];
        return;
    }

    // need to cut on the left or on the right ?
    std::size_t max_line_size = 0;
    for( const std::string &line : complete_lines )
        max_line_size = std::max( max_line_size, line.size() );
    std::size_t glo_beg_in_line = 0;
    std::size_t glo_end_in_line = max_line_size;
    if ( max_line_size > style.max_line_size ) {
        std::size_t mid = ( cols[ 1 ] + cols[ 2 ] ) / 2;
        // we have to cut the left part ?
        if ( mid > style.max_line_size / 2 ) {
            // we also have to cut the right part ?
            if ( mid - style.max_line_size / 2 + style.max_line_size < max_line_size ) {
                glo_beg_in_line = mid - style.max_line_size / 2;
                glo_end_in_line = mid - style.max_line_size / 2 + style.max_line_size;
            } else {
                glo_beg_in_line = max_line_size - style.max_line_size;
                glo_end_in_line = max_line_size;
            }
        } else {
            glo_beg_in_line = 0;
            glo_end_in_line = style.max_line_size;
        }
    }


    // else, we go through the lines
    for( std::size_t i = 0, num_line = lines[ 0 ]; i < complete_lines.size(); ++i, ++num_line ) {
        if ( i )
            os << "\n";
        os << style.indented_info_prefix;

        bool cut_beg = glo_beg_in_line > 0;
        bool cut_end = glo_end_in_line < complete_lines[ i ].size();
        std::size_t beg_in_line = cut_beg ? glo_beg_in_line + style.underline_ellipsis.size() : 0;
        std::size_t end_in_line = cut_end ? glo_end_in_line - style.underline_ellipsis.size() : complete_lines[ i ].size();

        if ( cut_beg )
            os << style.underline_ellipsis;
        os << complete_lines[ i ].substr( beg_in_line, end_in_line - beg_in_line );
        if ( cut_end )
            os << style.underline_ellipsis;

        if ( style.underline_extracts ) {
            os << "\n" << style.indented_info_prefix;
            for ( int num_col = beg_in_line - cut_beg * style.underline_ellipsis.size(); num_col < int( end_in_line + cut_end * style.underline_ellipsis.size() ); ++num_col ) {
                if ( std::tie( num_line, num_col ) < std::tie( lines[ 0 ], cols[ 0 ] ) )
                    os << ' ';
                else if ( std::tie( num_line, num_col ) < std::tie( lines[ 1 ], cols[ 1 ] ) )
                    os << style.underline_left_char;
                else if ( std::tie( num_line, num_col ) < std::tie( lines[ 2 ], cols[ 2 ] ) )
                    os << style.underline_middle_char;
                else if ( std::tie( num_line, num_col ) < std::tie( lines[ 3 ], cols[ 3 ] ) )
                    os << style.underline_right_char;
                else
                    break;
            }
        }
    }
}

void LogProvenance::init_data_from_offsets( StrView content, const std::array<ST,4> &offsets ) {
    // find line and col numbers for each offset
    const char *b = content.data(), *pcol = b;
    ST line = 0;
    for( std::size_t i = 0; i < offsets.size(); ++i ) {
        const char *p = content.data() + offsets[ i ];
        for( ; b < p; ++b ) {
            if ( *b == '\n' ) {
                if ( i )
                    complete_lines.push_back( pcol, b );
                pcol = b + 1;
                ++line;
            }
        }

        lines[ i ] = line;
        cols[ i ] = p - pcol;
    }

    // last line
    while ( b < content.end() && *b != '\n' )
        ++b;
    complete_lines.push_back( pcol, b );

    for( std::string &complete_line : complete_lines )
        if ( complete_line.size() && complete_line.back() == '\r' )
            complete_line.pop_back();
}

std::string LogProvenance::extract( std::string cr ) const {
    if ( complete_lines.size() > 1 ) {
        std::string res = complete_lines[ 0 ].substr( cols[ 0 ] );
        for( unsigned i = 1; i < complete_lines.size() - 1; ++i )
            res += cr + complete_lines[ i ];
        res += cr + complete_lines.back().substr( 0, cols[ 3 ] );
        return res;
    }
    return complete_lines[ 0 ].substr( cols[ 0 ], cols[ 3 ] - cols[ 0 ] ); // + " " + std::to_string( offsets[ 0 ] ) + std::to_string( offsets[ 1 ] ) + std::to_string( offsets[ 2 ] ) + std::to_string( offsets[ 3 ] );
}

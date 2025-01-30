#pragma once

#include "../containers/Vec.h"
#include "StyleOfTextLog.h"

#include <iostream>
#include <array>

/**
  Store and construct provenance information for the log, as in messages like

  my_file.ppw:10:16: Found no valid surdefinition for `operator**`
    ...my_code( extract ** "a", ) + ...
                --------^^----
*/
class LogProvenance {
public:
    using            ST                    = unsigned;

    /**/             LogProvenance         ( StrView filename, StrView content, const std::array<ST,4> &offsets = { 1, 0, 0, 0 } ); ///< offsets are assumed to be sorted. They are considered as invalid if it's not the case
    /**/             LogProvenance         ( StrView filename = {}, const std::array<ST,4> &offsets = { 1, 0, 0, 0 } ); ///< offsets are assumed to be sorted. They are considered as invalid (and the file is not read) if it's not the case

    void             display_inline_extract( std::ostream &os, const StyleOfTextLog &style = {} ) const;
    void             display_text_extract  ( std::ostream &os, const StyleOfTextLog &style = {}, bool prefix_with_newline = false ) const;
    void             display_text_header   ( std::ostream &os, const StyleOfTextLog &style = {} ) const;
    Str              extract               ( std::string cr = "\n" ) const;
    operator         bool                  () const { return offsets[ 0 ] <= offsets[ 3 ]; }

    Vec<Str>         complete_lines;       ///<
    Str              filename;             ///< name of the file
    std::array<ST,4> offsets;              ///< from the beginning of the file
    std::array<ST,4> lines;                ///< from the beginning of the file (starts at 0)
    std::array<ST,4> cols;                 ///< starts at 0

private:
    void             init_data_from_offsets( StrView content, const std::array<ST,4> &offsets ); ///< offsets is assumed to be non empty, content is assumed to be != 0
};

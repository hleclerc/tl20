#include <tl/support/string/StrRef.h>
#include "catch_main.h"

USING_TL_NAMESPACE

TEST_CASE( "String ref", "" ) {
    CHECK( StrRef( ""         ).id == STRING_REF_         );
    CHECK( StrRef( "operator" ).id == STRING_REF_operator );

    CHECK( StrRef( "smurf"    ).id == StrRef( "smurf" ).id );
    CHECK( StrRef( "smurf"    ).id != StrRef( "surf"  ).id );
}

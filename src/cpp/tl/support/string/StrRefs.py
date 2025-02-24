base_strings = [
    "",
    "operator",
]

def as_define( bs ):
    return bs

def as_str( bs ):
    return bs

def insert( filename, txt_to_insert, beg_txt = "// beg base string refs", end_txt = "// end base string refs" ):
    with open( filename, "r" ) as finp:
        txt = finp.read()
    beg = txt.find( beg_txt )
    end = txt.find( end_txt )

    with open( filename, "w" ) as fout:
        fout.write( txt[ : beg + len( beg_txt ) ] + "\n" )
        fout.write( txt_to_insert )
        fout.write( txt[ end : ] )

def write_cpp( filename = "StrRef.cpp" ):
    txt =  "static std::map<Str,PI32> id_map = {\n"
    for n, bs in enumerate( base_strings ):
        txt += f'    {{ "{ as_str( bs ) }", { n } }},\n'
    txt += "};\n"

    txt += "static Vec<StrView> str_vec = {\n"
    for n, bs in enumerate( base_strings ):
        txt += f'    "{ as_str( bs ) }",\n'
    txt += "};\n"

    insert( filename, txt )


def write_h( filename = "StrRef.h" ):
    txt = ""
    for n, bs in enumerate( base_strings ):
        txt += f"#define STRING_REF_{ as_define( bs ) } { n }\n"
    insert( filename, txt )


write_cpp()
write_h()

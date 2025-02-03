#pragma once

#include "../support/memory/BumpPointerPool.h"
#include "../support/Displayer.h"
#include "../support/log/Log.h"
#include "AstWriter.h"

#include "PreAst/Module.h"
#include "tl/parse/PreAst/Scope.h"

BEG_TL_NAMESPACE
class TlToken;

/** 
    node of a "pre ast" graph, used to make a final Ast for tokens
*/
class TlPreAstFromToken {
public:
    /**/             TlPreAstFromToken( Log &log, BumpPointerPool &pool );

    void             display          ( Displayer &ds ) const;
    void             write            ( AstWriter &aw ) const;

    void             push             ( TlToken *token );

private:    
    PreAst::Node*    make_node        ( TlToken *token );

    PreAst::Scope*   current_scope;
    PreAst::Module*  module;
    BumpPointerPool& pool;
    Log&             log;
};

END_TL_NAMESPACE

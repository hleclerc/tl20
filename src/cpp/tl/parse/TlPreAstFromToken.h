#pragma once

#include "../support/memory/BumpPointerPool.h"
#include "../support/Displayer.h"
#include "../support/log/Log.h"
#include "AstWriter.h"

#include "PreAst/Node_Variable.h"
#include "PreAst/FuncInfo.h"
#include "PreAst/Module.h"
#include "PreAst/Scope.h"
#include "PreAst/Arg.h"

BEG_TL_NAMESPACE
class TlToken;

/** 
    node of a "pre ast" graph, used to make a final Ast for tokens
*/
class TlPreAstFromToken {
public:
    /**/              TlPreAstFromToken( Log &log, BumpPointerPool &pool );
 
    void              display          ( Displayer &ds ) const;
    void              write            ( AstWriter &aw ) const;
 
    void              push             ( TlToken *token );
 
private:     
    using             Variables        = Vec<PreAst::Node_Variable *>;
 
    PreAst::Node*     make_variable    ( PreAst::Scope *scope, TlToken *token );
    PreAst::Node*     make_string      ( PreAst::Scope *scope, TlToken *token );
    PreAst::Node*     make_call        ( PreAst::Scope *scope, TlToken *token );
    PreAst::Node*     make_node        ( PreAst::Scope *scope, TlToken *token );
 
    PreAst::Arg       make_arg         ( PreAst::Scope *scope, TlToken *token );

    Variables         variables;       ///< list of all the variables (updated after the first phase)
    PreAst::FuncInfo* func_info;       ///< info for special funcs (like `def`, `:=`, ...)
    PreAst::Module*   module;
    BumpPointerPool&  pool;
    Log&              log;
};

END_TL_NAMESPACE

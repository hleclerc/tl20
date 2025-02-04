#pragma once

#include "../support/memory/BumpPointerPool.h"
#include "../support/Displayer.h"
#include "../support/log/Log.h"
#include "AstWriter.h"

#include "PreAst/Node_Variable.h"
#include "PreAst/VarDecl.h"
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
    /**/              TlPreAstFromToken ( Log &log, BumpPointerPool &pool );
  
    void              display           ( Displayer &ds ) const;
    void              write             ( AstWriter &aw ) const;
  
    void              push              ( TlToken *token );
  
private:      
    using             VariableRefs      = Vec<std::pair<PreAst::Scope *,PI>>;
 
    PreAst::Node*     make_node_variable( PreAst::Scope *scope, TlToken *token );
    PreAst::Node*     make_node_string  ( PreAst::Scope *scope, TlToken *token );
    PreAst::Node*     make_node_call    ( PreAst::Scope *scope, TlToken *token );
    PreAst::Node*     make_node         ( PreAst::Scope *scope, TlToken *token );
  
    PreAst::VarDecl*  make_var_decl     ( PreAst::Scope *scope, TlToken *token, bool func_by_default );
    PreAst::Block*    make_block        ( PreAst::Scope *scope, TlToken *token );
    PreAst::Arg       make_arg          ( PreAst::Scope *scope, TlToken *token );
 
    VariableRefs      variable_refs;    ///< list of variables for the second phase
    PreAst::Module    module;
    BumpPointerPool&  pool;
    Log&              log;
};

END_TL_NAMESPACE

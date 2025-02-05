#pragma once

#include "../support/memory/BumpPointerPool.h"
#include "../support/Displayer.h"
#include "../support/log/Log.h"
#include "Ast::Writer.h"

#include "Pst/Node_Variable.h"
#include "Pst/VarDecl.h"
#include "Pst/Module.h"
#include "Pst/Scope.h"
#include "Pst/Arg.h"

BEG_TL_NAMESPACE
class Tok::Node;

/** 
    node of a "pre ast" graph, used to make a final Ast for tokens
*/
class TlPreAstFromToken {
public:
    /**/              TlPreAstFromToken ( Log &log, BumpPointerPool &pool );
  
    void              display           ( Displayer &ds ) const;
    void              write             ( Ast::Writer &aw ) const;
  
    void              push              ( Tok::Node *token );
  
private:      
    using             VariableRefs      = Vec<Pst::Node_Variable *>;
 
    Pst::Node*     make_node_variable( Pst::Scope *scope, Tok::Node *token );
    Pst::Node*     make_node_string  ( Pst::Scope *scope, Tok::Node *token );
    Pst::Node*     make_node_call    ( Pst::Scope *scope, Tok::Node *token );
    Pst::Node*     make_node         ( Pst::Scope *scope, Tok::Node *token );
  
    Pst::VarDecl*  make_var_decl     ( Pst::Scope *scope, Tok::Node *token, bool func_by_default );
    Pst::Block*    make_block        ( Pst::Scope *scope, Tok::Node *token );
    Pst::Arg       make_arg          ( Pst::Scope *scope, Tok::Node *token );
 
    VariableRefs      variable_refs;    ///< list of variables for the second phase
    Pst::Module    module;
    BumpPointerPool&  pool;
    Log&              log;
};

END_TL_NAMESPACE

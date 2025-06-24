#pragma once

#include "../support/memory/BumpPointerPool.h"
#include "../support/Displayer.h"
#include "../support/log/Log.h"
#include "Tok/TNode.h"

#include "Pst/Node_Variable.h"
#include "Pst/VarDecl.h"
#include "Pst/Scope.h"
#include "Pst/Block.h"
#include "Pst/Arg.h"

BEG_TL_NAMESPACE

/** 
    node of a "pre ast" graph, used to make a final Ast with full information on catched variables
*/
class PstFromTok {
public:
    /**/               PstFromTok        ( Log &log );
   
    void               parse             ( Tok::TNode *token );
    void               write             ( Ast &ast ) const;
 
    void               display           ( Displayer &ds ) const;
    Pst::PNode*        root              () const;
     
private:       
    using              VariableRefs      = Vec<Pst::Node_Variable *>;
 
    Pst::PNode*        make_node_variable( Pst::Scope *scope, Tok::TNode *token );
    Pst::PNode*        make_node_module  ( Pst::Scope *scope, Tok::TNode *token );
    Pst::PNode*        make_node_string  ( Pst::Scope *scope, Tok::TNode *token );
    Pst::PNode*        make_node_call    ( Pst::Scope *scope, Tok::TNode *token );
    Pst::PNode*        make_node         ( Pst::Scope *scope, Tok::TNode *token );
     
    Pst::VarDecl*      make_var_decl     ( Pst::Scope *scope, Tok::TNode *token, bool func_by_default );
    Pst::Block*        make_block        ( Pst::Scope *scope, Tok::TNode *token, Vec<GString> *global_variables = nullptr );
    Pst::Arg           make_arg          ( Pst::Scope *scope, Tok::TNode *token );
 
    VariableRefs       variable_refs;    ///< list of variables for the second phase
    Pst::Scope         base_scope;       ///<
    Pst::PNode*        root_node;        ///<
    BumpPointerPool    pool;
    Log&               log;
};

END_TL_NAMESPACE

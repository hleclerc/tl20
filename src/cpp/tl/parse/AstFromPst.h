#pragma once

#include "../support/memory/BumpPointerPool.h"
#include "../support/Displayer.h"
#include "../support/log/Log.h"
#include "Ast/Writer.h"
#include "Ast/Crepr.h"
#include "Pst/Node.h"

BEG_TL_NAMESPACE

/** 
    make an Ast from a Pst tree
*/
class AstFromPst {
public:
    /**/              AstFromPst        ( Log &log );
  
    void              parse             ( Pst::Node *node );
    Ast::CRepr        crepr             ();

    void              display           ( Displayer &ds ) const;
  
private:      
    Log&              log;
};

END_TL_NAMESPACE

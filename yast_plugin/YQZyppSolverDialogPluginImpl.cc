/*---------------------------------------------------------------------\
|								       |
|		       __   __	  ____ _____ ____		       |
|		       \ \ / /_ _/ ___|_   _|___ \		       |
|			\ V / _` \___ \ | |   __) |		       |
|			 | | (_| |___) || |  / __/		       |
|			 |_|\__,_|____/ |_| |_____|		       |
|								       |
|				core system			       |
|						     (c) SuSE Linux AG |
\----------------------------------------------------------------------/

  File:		YQZyppSolverDialogPluginImpl.cc

  Author:	Stefan Schubert

/-*/

#include "YQZyppSolverDialogPluginImpl.h"

#define YUILogComponent "qt-solver-dialog"
#include <YUILog.h>
#include "QZyppSolverDialog.h"

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : YQZyppSolverDialogPluginStub::createZyppSolverDialog
//	METHOD TYPE : YWidget
//
//	DESCRIPTION : 
//
extern "C" {
YQZyppSolverDialogPluginImpl ZYPPDIALOGP;
}

bool YQZyppSolverDialogPluginImpl::createZyppSolverDialog( const zypp::PoolItem item )
{
    yuiMilestone() << "calling createZyppSolverDialog " << endl;

    QZyppSolverDialog *dialog = new QZyppSolverDialog(item);
    dialog->setMinimumSize ( 700, 700 );
    dialog->show();    
    
    return true;
}



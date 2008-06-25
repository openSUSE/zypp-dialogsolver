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

  File:		YQZyppSolverDialogPluginImpl.h

  Author:	Stefan Schubert <schubi@suse.de>


/-*/

#ifndef YQZyppSolverDialogPluginImpl_h
#define YQZyppSolverDialogPluginImpl_h

#include "YQZyppSolverDialogPluginIf.h"
#include <zypp/PoolItem.h>

class YQZyppSolverDialogPluginImpl : public YQZyppSolverDialogPluginIf
{

  public:

    virtual ~YQZyppSolverDialogPluginImpl() {}

    virtual bool createZyppSolverDialog( const zypp::PoolItem item );

};
#endif

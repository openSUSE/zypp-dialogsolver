#include <qpushbutton.h>
#include <qdialog.h>
#include <qinputdialog.h>
#include <qlayout.h>
#include <qmessagebox.h>
#include <stdio.h>

#include "QZyppSolverDialog.h"
#include "solvertree.h"
#include "getText.h"
#include "zypp/ZYppFactory.h"
#include "zypp/ResFilters.h"
#include "zypp/base/Algorithm.h"

using namespace zypp;

QZyppSolverDialog::QZyppSolverDialog(zypp::solver::detail::Resolver_Ptr r)
      : QDialog(0,"Solvertree",true)
      , resolver (r)
      , solvertree(0)
{
    QHBoxLayout* layout = new QHBoxLayout (this);
    solvertree = new SolverTree(this, resolver);
    layout->addWidget( solvertree->getView());

    if (resolver == NULL
	|| (resolver->problems()).size() > 0 ) {
	QMessageBox::critical( 0,
			       i18n("Critical Error") ,
			       i18n("No valid solver result"));
    }
}


struct UndoTransact : public resfilter::PoolItemFilterFunctor
{
    ResStatus::TransactByValue resStatus;
    UndoTransact ( const ResStatus::TransactByValue &status)
	:resStatus(status)
    { }

    bool operator()( PoolItem item )		// only transacts() items go here
    {
	item.status().resetTransact( resStatus );// clear any solver/establish transactions
	return true;
    }
};


QZyppSolverDialog::QZyppSolverDialog(const zypp::PoolItem item)
      : QDialog(0,"Solvertree",true)
      , resolver (NULL)
      , solvertree(0)
{
    zypp::ResPool pool( zypp::getZYpp()->pool() );
    pool.proxy().saveState(); // Save old pool
    const QCursor oldCursor = cursor ();
    setCursor (Qt::WaitCursor); 		    
    
    // resetting all selections
    UndoTransact resetting (ResStatus::USER);
    invokeOnEach ( pool.begin(), pool.end(),
		   resfilter::ByTransact( ),			// Resetting all transcations
		   functor::functorRef<bool,PoolItem>(resetting) );

    // set the selected item for installation only
    item.status().setToBeInstalled( ResStatus::USER);
    
    // and resolve		    
    resolver = new zypp::solver::detail::Resolver( pool );
    resolver->resolvePool();
    
    // show the results
    QHBoxLayout* layout = new QHBoxLayout (this);
    solvertree = new SolverTree(this, resolver);
    layout->addWidget( solvertree->getView());
    selectItem(item);
    pool.proxy().restoreState(); // Restore old state

    if (resolver == NULL
	|| (resolver->problems()).size() > 0 ) {
	QMessageBox::critical( 0,
			       i18n("Critical Error") ,
			       i18n("No valid solver result"));
    }
    setCursor (oldCursor);     
}


QZyppSolverDialog::~QZyppSolverDialog()
{
}


void QZyppSolverDialog::selectItem(const zypp::PoolItem item) {
    solvertree->selectItem(item);
};


#include "QZyppSolverDialog.moc"

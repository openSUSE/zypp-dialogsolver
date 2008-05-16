#include <qpushbutton.h>
#include <qdialog.h>
#include <qinputdialog.h>
#include <qlayout.h>
#include <qmessagebox.h>
#include <stdio.h>

#include "QZyppSolverDialog.h"
#include "solvertree.h"

#define i18n(MSG) QString(MSG)

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


QZyppSolverDialog::~QZyppSolverDialog()
{
}

void QZyppSolverDialog::selectItem(const zypp::PoolItem item) {
    solvertree->selectItem(item);
};


#include "QZyppSolverDialog.moc"

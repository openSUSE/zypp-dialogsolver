#include <qpushbutton.h>
#include <qdialog.h>
#include <qinputdialog.h>
#include <qlayout.h>
#include <stdio.h>

#include "QZyppSolverDialog.h"
#include "solvertree.h"


QZyppSolverDialog::QZyppSolverDialog(zypp::solver::detail::Resolver_Ptr r)
      : QDialog(0,"Solvertree",true)
      , resolver (r)
{
    QHBoxLayout* layout = new QHBoxLayout (this);
    SolverTree *solvertree = new SolverTree(this, resolver);
    layout->addWidget( solvertree->getView());
}


QZyppSolverDialog::~QZyppSolverDialog()
{
}

#include "QZyppSolverDialog.moc"

#ifndef QZYPPSOLVERDIALOG_H
#define QZYPPSOLVERDIALOG_H

#include <qdialog.h>
#include "zypp/solver/detail/Resolver.h"

class SolverTree;

  ///////////////////////////////////////////////////////////////////
  //
  //	CLASS NAME : QZyppSolverDialog
  //
  /**
   * Modal QDialog class which shows the solver result in
   * a graphical view.
   */

class QZyppSolverDialog : public QDialog{
    Q_OBJECT

protected:
    zypp::solver::detail::Resolver_Ptr resolver;
    SolverTree *solvertree;

public:
    /** Constructor
     * \param Resolver pointer of a valid solver run
    */    
     QZyppSolverDialog(zypp::solver::detail::Resolver_Ptr r = NULL);
    
    /** ctor
    */    
    ~QZyppSolverDialog();

    /** Selecting one item in the solvertree
     * \param \ref item which will be selected
    */        
    void selectItem(const zypp::PoolItem_Ref item);
    
private:
    QZyppSolverDialog&
    operator = (const QZyppSolverDialog &zyppSolverDialog);
    QZyppSolverDialog(const QZyppSolverDialog &zyppSolverDialog);

};

#endif

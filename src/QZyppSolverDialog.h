#ifndef QZYPPSOLVERDIALOG_H
#define QZYPPSOLVERDIALOG_H

#include <qdialog.h>
#include "zypp/solver/detail/Resolver.h"

class SolverTree;

class QZyppSolverDialog : public QDialog{
    Q_OBJECT

protected:
    zypp::solver::detail::Resolver_Ptr resolver;
    SolverTree *solvertree;

public:
     QZyppSolverDialog(zypp::solver::detail::Resolver_Ptr r = NULL);
    ~QZyppSolverDialog();

    void selectItem(const zypp::PoolItem_Ref item);
private:
    QZyppSolverDialog&
    operator = (const QZyppSolverDialog &zyppSolverDialog);
    QZyppSolverDialog(const QZyppSolverDialog &zyppSolverDialog);

};

#endif

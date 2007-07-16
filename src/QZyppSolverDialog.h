#ifndef QZYPPSOLVERDIALOG_H
#define QZYPPSOLVERDIALOG_H

#include <qdialog.h>
#include "zypp/solver/detail/Resolver.h"


class QZyppSolverDialog : public QDialog{
    Q_OBJECT

protected:    
    zypp::solver::detail::Resolver_Ptr resolver;

public:
    QZyppSolverDialog(zypp::solver::detail::Resolver_Ptr r = NULL);
    ~QZyppSolverDialog();


};

#endif

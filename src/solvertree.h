/***************************************************************************
 *   Copyright (C) 2005-2007 by Rajko Albrecht                             *
 *   ral@alwins-world.de                                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/
#ifndef SOLVERTREE_H
#define SOLVERTREE_H

#include "zypp/Resolver.h"
#include "resgraphview.h"

class StreeData;
class QWidget;

/**
	@author Rajko Albrecht <ral@alwins-world.de>
*/
class SolverTree{
public:
    SolverTree( QWidget*treeParent,
		zypp::solver::detail::Resolver_Ptr r = NULL);
    virtual ~SolverTree();

    bool isValid()const;
    QWidget*getView();
    void selectItem(const zypp::PoolItem item);

protected:
    bool m_Valid;
    zypp::solver::detail::Resolver_Ptr resolver;
    StreeData*m_Data;

    void buildTree ( StreeData *data, ResGraphView::tlist &childList, const zypp::PoolItem item, int &id);

};

#endif

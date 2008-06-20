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

#include <iostream>
#include <fstream>
#include <sstream>
#include <streambuf>


#include "solvertree.h"
#include "restreewidget.h"
#include "resgraphview.h"
#include <qwidget.h>
#include <qdatetime.h>
#include <qstring.h>

#include <zypp/PoolItem.h>
#include <zypp/Resolver.h>
#include <zypp/Resolvable.h>

class StreeData
{
public:
    StreeData();
    virtual ~StreeData();

    QTime m_stopTick;

    ResTreeWidget *m_TreeDisplay;

};

StreeData::StreeData()
{
    m_TreeDisplay = 0;
}

StreeData::~StreeData()
{
}



SolverTree::SolverTree( QWidget*treeParent,
			zypp::solver::detail::Resolver_Ptr r)
    :m_Valid(false)
     ,resolver(r)
{
    m_Data = new StreeData;

    m_Data->m_stopTick.restart();
    m_Data->m_TreeDisplay=new ResTreeWidget(treeParent, resolver);
    m_Data->m_TreeDisplay->setMinimumSize ( 700, 700 );
    m_Data->m_TreeDisplay->buildTree();
    
    m_Valid=true;
}

SolverTree::~SolverTree()
{
    delete m_Data;
}

void SolverTree::selectItem(const zypp::PoolItem item) {
    m_Data->m_TreeDisplay->selectItem(item);
}


bool SolverTree::isValid()const
{
    return m_Valid;
}


QWidget*SolverTree::getView()
{
    return m_Data->m_TreeDisplay;
}


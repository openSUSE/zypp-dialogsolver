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

    if (resolver != NULL) {
	int id = 0;
	for (zypp::ResPool::const_iterator it = resolver->pool().begin();
	     it != resolver->pool().end();
	     ++it)
	{ // find all root items and generate 
	    if (it->status().isToBeInstalled()) {
		
		zypp::ResObject::constPtr r = it->resolvable();		
		bool rootfound = false;
		zypp::solver::detail::ItemCapKindList isInstalledList = resolver->isInstalledBy (*it);		
		if (isInstalledList.empty()) {
		    rootfound = true;
		} else {
		    rootfound = true;
		    for (zypp::solver::detail::ItemCapKindList::const_iterator isInstall = isInstalledList.begin();
			 isInstall != isInstalledList.end(); isInstall++) {
			if (isInstall->initialInstallation) {
			    rootfound = false;
			}
		    }
		}

		if (rootfound) {
		    QString idStr = QString( "%1" ).arg( id++ );		    
		    m_Data->m_TreeDisplay->m_RevGraphView->m_Tree[idStr].item = *it;

		    // we have found a root; collect all trees
		    buildTree ( m_Data, m_Data->m_TreeDisplay->m_RevGraphView->m_Tree[idStr].targets, *it, id);
		}
	    }	
	}
    }
    
    m_Valid=true;
    m_Data->m_TreeDisplay->dumpRevtree();
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


void SolverTree::buildTree ( StreeData *data,  ResGraphView::tlist &childList, const zypp::PoolItem item, int &id) {
    // generate the branches
    zypp::solver::detail::ItemCapKindList installList = resolver->installs (item);
    for (zypp::solver::detail::ItemCapKindList::const_iterator it = installList.begin();
	 it != installList.end(); it++) {
	if (it->initialInstallation) {
	    // This item will REALLY triggered by the given root item (not only due required dependencies)
	    QString idStr = QString( "%1" ).arg( id++ );

	    childList.append(ResGraphView::targetData(idStr));		    
	    data->m_TreeDisplay->m_RevGraphView->m_Tree[idStr].item=it->item;
	    data->m_TreeDisplay->m_RevGraphView->m_Tree[idStr].dueto = *it;

	    alreadyHitItems.insert (item);

	    // we have found a root; collect all trees
	    if (alreadyHitItems.find(it->item) == alreadyHitItems.end())
		buildTree ( data, data->m_TreeDisplay->m_RevGraphView->m_Tree[idStr].targets, it->item, id); 		    
	}
    }    
}


QWidget*SolverTree::getView()
{
    return m_Data->m_TreeDisplay;
}


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

    unsigned count = 0;
    m_Data->m_stopTick.restart();
    m_Data->m_TreeDisplay=new ResTreeWidget(treeParent);
    m_Data->m_TreeDisplay->setMinimumSize ( 700, 700 );

    if (resolver == NULL) { // testcase

	QString n12 = "product2";
	m_Data->m_TreeDisplay->m_RevGraphView->m_Tree[n12].name="SuSE Linux 10.2";
	m_Data->m_TreeDisplay->m_RevGraphView->m_Tree[n12].kind="Product";
	m_Data->m_TreeDisplay->m_RevGraphView->m_Tree[n12].source="nfs:://mounts/dist/....";
	m_Data->m_TreeDisplay->m_RevGraphView->m_Tree[n12].version="4.0";
	m_Data->m_TreeDisplay->m_RevGraphView->m_Tree[n12].description="Opensuse 10.2";

	for (count=1; count <= 4 ; ++count) {    

	    QString n2;

	    n2.setNum(count+1000);

	    m_Data->m_TreeDisplay->m_RevGraphView->m_Tree[n12].targets.append(ResGraphView::targetData(n2));

	    m_Data->m_TreeDisplay->m_RevGraphView->m_Tree[n2].name="Pattern " + n2;
	    m_Data->m_TreeDisplay->m_RevGraphView->m_Tree[n2].kind="Pattern";
	    m_Data->m_TreeDisplay->m_RevGraphView->m_Tree[n2].source="nfs:://mounts/dist/....";
	    m_Data->m_TreeDisplay->m_RevGraphView->m_Tree[n2].version="1.0";
	    m_Data->m_TreeDisplay->m_RevGraphView->m_Tree[n2].description="Patterndescription for " + n2;


	    for (int count2=1; count2 <= 4 ; ++count2) {    

		QString n3;

		n3.setNum(count*10+count2+1000);
		m_Data->m_TreeDisplay->m_RevGraphView->m_Tree[n2].targets.append(ResGraphView::targetData(n3));

		m_Data->m_TreeDisplay->m_RevGraphView->m_Tree[n3].name="Package " + n3;
		m_Data->m_TreeDisplay->m_RevGraphView->m_Tree[n3].kind="Package";
		m_Data->m_TreeDisplay->m_RevGraphView->m_Tree[n3].source="nfs:://mounts/dist/....";
		m_Data->m_TreeDisplay->m_RevGraphView->m_Tree[n3].version="1.0";
		m_Data->m_TreeDisplay->m_RevGraphView->m_Tree[n3].description="Packagedescription for " + n3;

		for (int count3=1; count3 <= count2 ; ++count3) {    

		    QString n4;

		    n4.setNum(count*100+count2*10+count3+1000);
		    m_Data->m_TreeDisplay->m_RevGraphView->m_Tree[n3].targets.append(ResGraphView::targetData(n4));

		    m_Data->m_TreeDisplay->m_RevGraphView->m_Tree[n4].name="Package " + n4;
		    m_Data->m_TreeDisplay->m_RevGraphView->m_Tree[n4].kind="Package";
		    m_Data->m_TreeDisplay->m_RevGraphView->m_Tree[n4].source="nfs:://mounts/dist/....";
		    m_Data->m_TreeDisplay->m_RevGraphView->m_Tree[n4].version="1.0";
		    m_Data->m_TreeDisplay->m_RevGraphView->m_Tree[n4].description="Packagedescription for " + n4;

		}
	    }
	}	

	
	QString n1 = "product";
	m_Data->m_TreeDisplay->m_RevGraphView->m_Tree[n1].name="SuSE SLES10";
	m_Data->m_TreeDisplay->m_RevGraphView->m_Tree[n1].kind="Product";
	m_Data->m_TreeDisplay->m_RevGraphView->m_Tree[n1].source="nfs:://mounts/dist/....";
	m_Data->m_TreeDisplay->m_RevGraphView->m_Tree[n1].version="4.0";
	m_Data->m_TreeDisplay->m_RevGraphView->m_Tree[n1].description="Enterprise Server 10";
    
	for (count=1; count <= 4 ; ++count) {    

	    QString n2;

	    n2.setNum(count);

	    m_Data->m_TreeDisplay->m_RevGraphView->m_Tree[n1].targets.append(ResGraphView::targetData(n2));

	    m_Data->m_TreeDisplay->m_RevGraphView->m_Tree[n2].name="Pattern " + n2;
	    m_Data->m_TreeDisplay->m_RevGraphView->m_Tree[n2].kind="Pattern";
	    m_Data->m_TreeDisplay->m_RevGraphView->m_Tree[n2].source="nfs:://mounts/dist/....";
	    m_Data->m_TreeDisplay->m_RevGraphView->m_Tree[n2].version="1.0";
	    m_Data->m_TreeDisplay->m_RevGraphView->m_Tree[n2].description="Patterndescription for " + n2;


	    for (int count2=1; count2 <= 4 ; ++count2) {    

		QString n3;

		n3.setNum(count*10+count2);
		m_Data->m_TreeDisplay->m_RevGraphView->m_Tree[n2].targets.append(ResGraphView::targetData(n3));

		m_Data->m_TreeDisplay->m_RevGraphView->m_Tree[n3].name="Package " + n3;
		m_Data->m_TreeDisplay->m_RevGraphView->m_Tree[n3].kind="Package";
		m_Data->m_TreeDisplay->m_RevGraphView->m_Tree[n3].source="nfs:://mounts/dist/....";
		m_Data->m_TreeDisplay->m_RevGraphView->m_Tree[n3].version="1.0";
		m_Data->m_TreeDisplay->m_RevGraphView->m_Tree[n3].description="Packagedescription for " + n3;

		for (int count3=1; count3 <= count2 ; ++count3) {    

		    QString n4;

		    n4.setNum(count*100+count2*10+count3);
		    m_Data->m_TreeDisplay->m_RevGraphView->m_Tree[n3].targets.append(ResGraphView::targetData(n4));

		    m_Data->m_TreeDisplay->m_RevGraphView->m_Tree[n4].name="Package " + n4;
		    m_Data->m_TreeDisplay->m_RevGraphView->m_Tree[n4].kind="Package";
		    m_Data->m_TreeDisplay->m_RevGraphView->m_Tree[n4].source="nfs:://mounts/dist/....";
		    m_Data->m_TreeDisplay->m_RevGraphView->m_Tree[n4].version="1.0";
		    m_Data->m_TreeDisplay->m_RevGraphView->m_Tree[n4].description="Packagedescription for " + n4;

		}	    
	    }
	}
    } else {
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
		    zypp::ResObject::constPtr res = it->resolvable();
		    QString idStr = QString( "%1" ).arg( id++ );		    

		    m_Data->m_TreeDisplay->m_RevGraphView->m_Tree[idStr].name=res->name();
		    m_Data->m_TreeDisplay->m_RevGraphView->m_Tree[idStr].kind=res->kind().asString();
		    m_Data->m_TreeDisplay->m_RevGraphView->m_Tree[idStr].source="";
		    m_Data->m_TreeDisplay->m_RevGraphView->m_Tree[idStr].version=res->edition().asString()+"."+res->arch().asString();
		    m_Data->m_TreeDisplay->m_RevGraphView->m_Tree[idStr].description=res->description();
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


bool SolverTree::isValid()const
{
    return m_Valid;
}


void SolverTree::buildTree ( StreeData *data,  ResGraphView::tlist &childList, const zypp::PoolItem_Ref item, int &id) {
    // generate the branches
    zypp::solver::detail::ItemCapKindList installList = resolver->installs (item);
    for (zypp::solver::detail::ItemCapKindList::const_iterator it = installList.begin();
	 it != installList.end(); it++) {
	if (it->initialInstallation) {
	    // This item will REALLY triggered by the given root item (not only due required dependencies)
	    QString idStr = QString( "%1" ).arg( id++ );

	    childList.append(ResGraphView::targetData(idStr));		    
	    data->m_TreeDisplay->m_RevGraphView->m_Tree[idStr].name=it->item->name();
	    data->m_TreeDisplay->m_RevGraphView->m_Tree[idStr].kind=it->item->kind().asString();
	    data->m_TreeDisplay->m_RevGraphView->m_Tree[idStr].source="";
	    data->m_TreeDisplay->m_RevGraphView->m_Tree[idStr].version=it->item->edition().asString()+"."+it->item->arch().asString();
	    data->m_TreeDisplay->m_RevGraphView->m_Tree[idStr].description=it->item->description();

	    // we have found a root; collect all trees
	    buildTree ( data, data->m_TreeDisplay->m_RevGraphView->m_Tree[idStr].targets, it->item, id); 		    
	}
    }    
}


QWidget*SolverTree::getView()
{
    return m_Data->m_TreeDisplay;
}


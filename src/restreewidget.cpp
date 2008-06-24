/***************************************************************************
 *   Copyright (C) 2006-2007 by Rajko Albrecht                             *
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
#include "restreewidget.h"

#include <qvariant.h>
#include <qsplitter.h>
#include <q3textbrowser.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qmessagebox.h>
#include <qwhatsthis.h>
#include <q3hbox.h>
#include <q3vbox.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qstringlist.h>
#include <qnamespace.h>
#include "resgraphview.h"
#include "zypp/Resolver.h"
#include "zypp/ZYppFactory.h"
#include "zypp/ResFilters.h"
#include "zypp/base/Algorithm.h"
#include "getText.h"

using namespace zypp;

/*
 *  Constructs a ResTreeWidget as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
ResTreeWidget::ResTreeWidget(QWidget* parent, zypp::solver::detail::Resolver_Ptr r,
			     const zypp::PoolItem item,
			     const char* name, Qt::WFlags fl) 
    : QWidget( parent, name, fl )
      ,resolver(r)
      ,root_item(item)
{
    _lastSelectedItem = "";    
    if ( !name )
        setName( "ResTreeWidget" );
    ResTreeWidgetLayout = new QVBoxLayout( this, 11, 6, "ResTreeWidgetLayout");

    m_Splitter = new QSplitter( this, "m_Splitter" );
    m_Splitter->setOrientation( Qt::Vertical );

    m_RevGraphView = new ResGraphView(m_Splitter, "m_RevGraphView" );
    m_RevGraphView->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 2, m_RevGraphView->sizePolicy().hasHeightForWidth() ) );
    connect(m_RevGraphView,SIGNAL(dispDetails(const QString&, const zypp::PoolItem)),this,SLOT(setDetailText(const QString&, const zypp::PoolItem)));

    descriptionBox = new Q3VBox( m_Splitter, "descriptionBox");
    descriptionBox->setSpacing (5);
    tabWidget = new QTabWidget( descriptionBox, "tabWidget");
    tabWidget->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)7, 0, 0, tabWidget->sizePolicy().hasHeightForWidth() ) );

    checkBox = new Q3HBox( descriptionBox, "checkBox");
    checkBox->setSpacing (5);
    showInstalled = new QCheckBox(i18n("show installed packages"), checkBox);
    showRecommend = new QCheckBox(i18n("show recommended packages"), checkBox);;
    showInstalled->setChecked(true);
    if (resolver->onlyRequires()) 
	showRecommend->setChecked(false);
    else
	showRecommend->setChecked(true);	
    
    searchBox = new Q3HBox( descriptionBox, "searchBox");
    searchBox->setSpacing (5);
    searchLabel = new QLabel (i18n("Search: "), searchBox);
    searchLabel->setMaximumSize( searchLabel->minimumSizeHint () );
    resolvableList = new QComboBox( true, searchBox);
    resolvableList->setAutoCompletion(true);

    m_Detailstext = new Q3TextBrowser( tabWidget, "m_Detailstext" );
    m_Detailstext->setResizePolicy( Q3TextBrowser::Manual );
    tabWidget->addTab( m_Detailstext, i18n("Description") );

    installListView = new Q3ListView( tabWidget, "installListView" );
    installListView->addColumn( i18n("Name") );
    installListView->addColumn( i18n("Version") );
    installListView->addColumn( i18n("Dependency") );
    installListView->addColumn( i18n("Kind") );
    installListView->setAllColumnsShowFocus( TRUE );    
    tabWidget->addTab( installListView, i18n("Needs") );

    installedListView = new Q3ListView( tabWidget, "installListView" );
    installedListView->addColumn( i18n("Name") );
    installedListView->addColumn( i18n("Version") );
    installedListView->addColumn( i18n("Dependency") );
    installedListView->addColumn( i18n("Kind") );
    installedListView->setAllColumnsShowFocus( TRUE );
    tabWidget->addTab( installedListView, i18n("Needed by") );

    connect( installedListView, SIGNAL( clicked( Q3ListViewItem* ) ), this, SLOT( itemSelected( Q3ListViewItem* ) ) );
    connect( installListView, SIGNAL( clicked( Q3ListViewItem* ) ), this, SLOT( itemSelected( Q3ListViewItem* ) ) );
    connect( resolvableList, SIGNAL( activated( const QString & ) ), this, SLOT( slotComboActivated( const QString & ) ) );
    connect( showRecommend, SIGNAL( stateChanged ( int )  ), this, SLOT( showRecommendChanged ( int ) ) );
    connect( showInstalled, SIGNAL( stateChanged ( int )  ), this, SLOT( showInstalledChanged ( int ) ) );
    
    ResTreeWidgetLayout->addWidget(m_Splitter);
    
}

/*
 *  Destroys the object and frees any allocated resources
 */
ResTreeWidget::~ResTreeWidget()
{
}

void ResTreeWidget::dumpRevtree()
{
    m_RevGraphView->dumpRevtree();
    if (m_RevGraphView
	&& resolvableList) {
	resolvableList->clear();
	// creating entries in the combobox
	ResGraphView::trevTree::ConstIterator it;
	QStringList stringList;
	for (it=m_RevGraphView->m_Tree.begin();
	     it!=m_RevGraphView->m_Tree.end();++it) {
	    zypp::PoolItem item = it.data().item;
	    QString itemString = item->name().c_str();
	    itemString += "-";
	    itemString += item->edition().asString().c_str();
	    itemString += ".";
	    itemString += item->arch().asString().c_str();
	    if (stringList.find(itemString) == stringList.end())
		stringList.append (itemString);
	}
	stringList.sort();
	resolvableList->insertStringList (stringList);
    }
    selectItem(_lastSelectedItem); // Show the selected item (Could be set via API meanwhile)
}

void ResTreeWidget::setDetailText(const QString& _s, const zypp::PoolItem item)
{
    if (resolver) {
	zypp::solver::detail::ItemCapKindList installList = resolver->installs (item);
	zypp::solver::detail::ItemCapKindList installedList = resolver->isInstalledBy (item);
	installListView->clear();
	installedListView->clear();

	for (zypp::solver::detail::ItemCapKindList::const_iterator iter = installedList.begin();
	     iter != installedList.end(); ++iter) {
	    QString edition = iter->item->edition().asString().c_str();
	    edition += ".";
	    edition += iter->item->arch().asString().c_str();

	    Q3ListViewItem *element = new Q3ListViewItem( installedListView,
							QString(iter->item->name().c_str()),
							edition,
							QString(iter->cap.asString().c_str()),
							QString(iter->capKind.asString().c_str()));
	    element = NULL;
	}

	if (item.status().staysInstalled()) {
	    // Items which are installed. So they are already satifying requirements of others
	    zypp::solver::detail::ItemCapKindList installedSatisfied = resolver->installedSatisfied(item);
	    for (zypp::solver::detail::ItemCapKindList::const_iterator iter = installedSatisfied.begin();
		 iter != installedSatisfied.end(); ++iter) {
		QString edition = iter->item->edition().asString().c_str();
		edition += ".";
		edition += iter->item->arch().asString().c_str();

		Q3ListViewItem *element = new Q3ListViewItem( installedListView,
							      QString(iter->item->name().c_str()),
							      edition,
							      QString(iter->cap.asString().c_str()),
							      QString(iter->capKind.asString().c_str()));
		element = NULL;
	    }
	}
	    
	for (zypp::solver::detail::ItemCapKindList::const_iterator iter = installList.begin();
	     iter != installList.end(); ++iter) {
	    QString edition = iter->item->edition().asString().c_str();
	    edition += ".";
	    edition += iter->item->arch().asString().c_str();
	    Q3ListViewItem *element = new Q3ListViewItem( installListView,
							QString(iter->item->name().c_str()),
							edition,  
							QString(iter->cap.asString().c_str()),
							QString(iter->capKind.asString().c_str()));
	    element = NULL;	    
	}
    }
    
    m_Detailstext->setText(_s);
    Q3ValueList<int> list = m_Splitter->sizes();
    if (list.count()!=2) return;
    if (list[1]==0) {
        int h = height();
        int th = h/10;
        list[0]=h-th;
        list[1]=th;
        m_Splitter->setSizes(list);
    }
}

void ResTreeWidget::slotComboActivated( const QString &s ) {
    selectItem(s);
}

void ResTreeWidget::selectItem(const QString & itemString) {
    _lastSelectedItem = itemString;
    m_RevGraphView->selectItem (itemString );
}

void ResTreeWidget::selectItem(const zypp::PoolItem item) {
    QString itemString = item->name().c_str();
    itemString += "-";
    itemString += item->edition().asString().c_str();
    itemString += ".";
    itemString += item->arch().asString().c_str();
    selectItem (itemString);
}

void ResTreeWidget::itemSelected( Q3ListViewItem* item) {
    if ( !item )
        return;
    item->setSelected( TRUE );
    item->repaint();
    selectItem (item->text( 0 )+"-"+item->text( 1 ) );
}

struct UndoTransact : public resfilter::PoolItemFilterFunctor
{
    ResStatus::TransactByValue resStatus;
    UndoTransact ( const ResStatus::TransactByValue &status)
       :resStatus(status)
    { }

    bool operator()( PoolItem item )           // only transacts() items go here
    {
       item.status().resetTransact( resStatus );// clear any solver/establish transactions
       return true;
    }
};


void ResTreeWidget::showRecommendChanged(int state) {
    zypp::ResPool pool( zypp::getZYpp()->pool() );
    pool.proxy().saveState(); // Save old pool
    const QCursor oldCursor = cursor ();
    setCursor (Qt::WaitCursor);
    pool.proxy().saveState(); // Save old pool
    bool saveRec = resolver->onlyRequires();
    
    if (root_item != PoolItem()) {
	// Make a solver run with the selected item
	// resetting all selections
	UndoTransact resetting (ResStatus::USER);
	invokeOnEach ( pool.begin(), pool.end(),
		       resfilter::ByTransact( ),                    // Resetting all transcations
		       functor::functorRef<bool,PoolItem>(resetting) );
 
	// set the selected item for installation only
	root_item.status().setToBeInstalled( ResStatus::USER);
    }

    // and resolve		    
    resolver = new zypp::solver::detail::Resolver( pool );
    if (!showRecommend->isChecked()) {
	resolver->setOnlyRequires(true);
    } else {
	resolver->setOnlyRequires(false);	
    }
    resolver->resolvePool();
    if (resolver == NULL
	|| (resolver->problems()).size() > 0 ) {
	QMessageBox::critical( 0,
			       i18n("Critical Error") ,
			       i18n("No valid solver result"));
    }

    // show result
    m_RevGraphView->init();
    buildTree();

    pool.proxy().restoreState(); // Restore old state
    resolver->setOnlyRequires(saveRec);
    setCursor (oldCursor);         
}


void ResTreeWidget::showInstalledChanged(int state) {
    if (root_item != PoolItem()) {
	// Make a solver run with the selected item
	showRecommendChanged(0);
    } else {
	// state will be regarded while establish the tree
	m_RevGraphView->init();
	buildTree();
    }
}


void ResTreeWidget::buildTree() {
    if (resolver != NULL) {
	alreadyHitItems.clear();
	int id = 0;
	if (root_item == PoolItem()) {
	    // Ask the pool which items has been selected for installation
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
			m_RevGraphView->m_Tree[idStr].item = *it;

			// we have found a root; collect all trees
			buildTreeBranch ( m_RevGraphView->m_Tree[idStr].targets, *it, id);
		    }
		}	
	    }
	} else {
	    // take the selected root item for "root"
	    QString idStr = QString( "%1" ).arg( id++ );		    
	    m_RevGraphView->m_Tree[idStr].item = root_item;

	    // collect all trees
	    buildTreeBranch ( m_RevGraphView->m_Tree[idStr].targets, root_item, id);
	}
    }
    dumpRevtree();
    selectItem(_lastSelectedItem);
}  
    
void ResTreeWidget::buildTreeBranch ( ResGraphView::tlist &childList, const zypp::PoolItem item, int &id) {
    // generate the branches for items which will really be installed
    zypp::solver::detail::ItemCapKindList installList = resolver->installs (item);
    for (zypp::solver::detail::ItemCapKindList::const_iterator it = installList.begin();
	 it != installList.end(); it++) {
	if (it->initialInstallation) {
	    // This item will REALLY triggered by the given root item (not only due required dependencies)
	    QString idStr = QString( "%1" ).arg( id++ );

	    childList.append(ResGraphView::targetData(idStr));		    
	    m_RevGraphView->m_Tree[idStr].item=it->item;
	    m_RevGraphView->m_Tree[idStr].dueto = *it;

	    alreadyHitItems.insert (item);

	    // we have found a root; collect all trees
	    if (alreadyHitItems.find(it->item) == alreadyHitItems.end())
		buildTreeBranch ( m_RevGraphView->m_Tree[idStr].targets, it->item, id); 		    
	}
    }
    
    if (showInstalled->isChecked()) {
	// generate the branches for items which are already installed
	zypp::solver::detail::ItemCapKindList satisfiedList = resolver->satifiedByInstalled (item);
	for (zypp::solver::detail::ItemCapKindList::const_iterator it = satisfiedList.begin();
	     it != satisfiedList.end(); it++) {
	    QString idStr = QString( "%1" ).arg( id++ );

	    childList.append(ResGraphView::targetData(idStr));		    
	    m_RevGraphView->m_Tree[idStr].item=it->item;
	    m_RevGraphView->m_Tree[idStr].dueto = *it;

	    alreadyHitItems.insert (item);
	}
    }
}



#include "restreewidget.moc"


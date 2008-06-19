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
#include <qwhatsthis.h>
#include <q3hbox.h>
#include <q3vbox.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qstringlist.h>
#include <qnamespace.h>
#include "resgraphview.h"
#include "zypp/Resolver.h"
#include "getText.h"

/*
 *  Constructs a ResTreeWidget as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
ResTreeWidget::ResTreeWidget(QWidget* parent, zypp::solver::detail::Resolver_Ptr r, const char* name, Qt::WFlags fl) 
    : QWidget( parent, name, fl )
      ,resolver(r)
{
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

#include "restreewidget.moc"


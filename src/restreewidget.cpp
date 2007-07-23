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
#include <qtextbrowser.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "resgraphview.h"

/*
 *  Constructs a ResTreeWidget as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
ResTreeWidget::ResTreeWidget(QWidget* parent, zypp::solver::detail::Resolver_Ptr r, const char* name, WFlags fl) 
    : QWidget( parent, name, fl )
      ,resolver(r)
{
    if ( !name )
        setName( "ResTreeWidget" );
    ResTreeWidgetLayout = new QVBoxLayout( this, 11, 6, "ResTreeWidgetLayout");

    m_Splitter = new QSplitter( this, "m_Splitter" );
    m_Splitter->setOrientation( QSplitter::Vertical );

    m_RevGraphView = new ResGraphView(m_Splitter, "m_RevGraphView" );
    m_RevGraphView->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 2, m_RevGraphView->sizePolicy().hasHeightForWidth() ) );
    connect(m_RevGraphView,SIGNAL(dispDetails(const QString&)),this,SLOT(setDetailText(const QString&)));

    tabWidget = new QTabWidget( m_Splitter, "tabwidget" );
    tabWidget->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)7, 0, 0, tabWidget->sizePolicy().hasHeightForWidth() ) );    

    m_Detailstext = new QTextBrowser( tabWidget, "m_Detailstext" );
    m_Detailstext->setResizePolicy( QTextBrowser::Manual );
    tabWidget->addTab( m_Detailstext, "Description" );
    

    installListView = new QListView( tabWidget, "installListView" );
    installListView->addColumn( "Name" );
    installListView->addColumn( "Version" );    
    tabWidget->addTab( installListView, "Needs" );

    installedListView = new QListView( tabWidget, "installListView" );
    installedListView->addColumn( "Name" );
    installedListView->addColumn( "Version" );    
    tabWidget->addTab( installedListView, "Needed by" );
    
    ResTreeWidgetLayout->addWidget(m_Splitter);
    clearWState( WState_Polished );
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
}

void ResTreeWidget::setDetailText(const QString&_s)
{
    m_Detailstext->setText(_s);
    QValueList<int> list = m_Splitter->sizes();
    if (list.count()!=2) return;
    if (list[1]==0) {
        int h = height();
        int th = h/10;
        list[0]=h-th;
        list[1]=th;
        m_Splitter->setSizes(list);
    }
}

#include "restreewidget.moc"


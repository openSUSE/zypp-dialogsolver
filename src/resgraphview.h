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
#ifndef RESGRAPHVIEW_H
#define RESGRAPHVIEW_H


#include <q3canvas.h>
#include <qfile.h>
#include <q3process.h>
#include <qmap.h>
#include <zypp/PoolItem.h>
#include <zypp/Resolver.h>
#include <zypp/Resolvable.h>


namespace svn {
    class LogEntry;
    class Client;
}
class RevisionTree;
class SolverTree;
class GraphTreeLabel;
class GraphViewTipSolver;
class GraphMark;
class PannerView;
class CContextListener;

/**
	@author Rajko Albrecht <ral@alwins-world.de>
*/
class ResGraphView : public Q3CanvasView
{
    Q_OBJECT
public:
    enum ZoomPosition { TopLeft, TopRight, BottomLeft, BottomRight, Auto };
    /* avoid large copy operations */
    friend class RevisionTree;
    friend class SolverTree;
    friend class ResTreeWidget;

    ResGraphView(QWidget * parent = 0, const char * name = 0, Qt::WFlags f = 0);
    virtual ~ResGraphView();

    void showText(const QString&s);
    void clear();
    void init();

    void beginInsert();
    void endInsert();

    struct targetData {
        QString key;
        targetData(const QString&n)
        {
            key = n;
        }
        targetData(){key="";}
    };
    typedef Q3ValueList<targetData> tlist;

    struct keyData {
	zypp::PoolItem item;
	zypp::solver::detail::ItemCapKind dueto; // initial item/capability
        tlist targets;
    };

    typedef QMap<QString, keyData> trevTree;

    QString toolTip(const QString&nodename,bool full=false)const;

    void setBasePath(const QString&);
    void dumpRevtree();
    void selectItem(const QString & itemString);

signals:
    void dispDetails(const QString&, const zypp::PoolItem);

public slots:
    virtual void contentsMovingSlot(int,int);
    virtual void zoomRectMoved(int,int);
    virtual void zoomRectMoveFinished();
    virtual void slotClientException(const QString&what);

protected slots:
    virtual void dotExit();

protected:
    Q3Canvas*m_Canvas;
    GraphMark*m_Marker;
    GraphTreeLabel*m_Selected;
    QFile*dotTmpFile;
    QString dotOutput;
    Q3Process*renderProcess;
    trevTree m_Tree;
    const QString&getLabelstring(const QString&nodeName);
    QColor getBgColor(const QString&nodeName)const;
    bool isRecommended(const QString&nodeName)const;
    bool isStart(const QString&nodeName)const;

    QMap<QString,GraphTreeLabel*> m_NodeList;
    QMap<QString,QString> m_LabelMap;

    int _xMargin,_yMargin;
    PannerView*m_CompleteView;
    double _cvZoomW;
    double _cvZoomH;    
    ZoomPosition m_LastAutoPosition;

    virtual bool event(QEvent *event);    
    virtual void resizeEvent(QResizeEvent*);
    virtual void contentsMousePressEvent ( QMouseEvent * e );
    virtual void contentsMouseReleaseEvent ( QMouseEvent * e );
    virtual void contentsMouseMoveEvent ( QMouseEvent*e);
    virtual void contentsContextMenuEvent(QContextMenuEvent*e);
    virtual void contentsMouseDoubleClickEvent ( QMouseEvent * e );

    bool _isMoving;
    QPoint _lastPos;

    bool _noUpdateZoomerPos;

    QString _lastSelectedItem;

private:
    void updateSizes(QSize s = QSize(0,0));
    void updateZoomerPos();
    void setNewDirection(int dir);
    void makeSelected(GraphTreeLabel*);
};

#endif

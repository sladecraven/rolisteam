/***************************************************************************
*	Copyright (C) 2007 by Romain Campioni                                  *
*	Copyright (C) 2009 by Renaud Guezennec                                 *
*   http://renaudguezennec.homelinux.org/accueil,3.html                    *
*                                                                          *
*   rolisteam is free software; you can redistribute it and/or modify      *
*   it under the terms of the GNU General Public License as published by   *
*   the Free Software Foundation; either version 2 of the License, or      *
*   (at your option) any later version.                                    *
*                                                                          *
*   This program is distributed in the hope that it will be useful,        *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
*   GNU General Public License for more details.                           *
*                                                                          *
*   You should have received a copy of the GNU General Public License      *
*   along with this program; if not, write to the                          *
*   Free Software Foundation, Inc.,                                        *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.              *
***************************************************************************/


#include <QtGui>

#include "MapFrame.h"
#include "colorselector.h"



MapFrame::MapFrame(Map *map)
    : SubMdiWindows(),m_map(map)
{

    m_widgetLayout = new QWidget;
    m_graphicView = new RGraphicsView(map);
    m_vlayout= new QVBoxLayout();
    m_hlayout = new QHBoxLayout();
    m_type = SubMdiWindows::MAP;


    m_vlayout->addStretch(1);
    m_vlayout->addWidget(m_graphicView);
    m_vlayout->addStretch(1);


    m_hlayout->addStretch(1);
    m_hlayout->addLayout(m_vlayout);
    m_hlayout->addStretch(1);
    setWindowTitle(m_map->mapTitle());
    m_graphicView->setGeometry(0,0,m_map->mapWidth(),map->mapHeight());
    m_widgetLayout->setLayout(m_hlayout);
    setWidget(m_widgetLayout);
    //setLayout(hlayout);
    m_maskPixmap = new QPixmap(m_graphicView->size());

    m_currentEditingMode=ColorSelector::NORMAL;

}


MapFrame::~MapFrame()
{
    delete m_widgetLayout;
    delete m_graphicView;
    delete m_hlayout;
    delete m_vlayout;
    delete m_maskPixmap;

}


void MapFrame::closeEvent(QCloseEvent *event)
{

    hide();
    event->ignore();
}


Map * MapFrame::map()
{
    return m_map;
}
int MapFrame::editingMode()
{
    return m_currentEditingMode;
}

void MapFrame::startMoving(QPoint position)
{
    startingPoint = position;
    horizontalStart = m_graphicView->horizontalScrollBar()->value();
    verticalStart = m_graphicView->verticalScrollBar()->value();
}


void MapFrame::Moving(QPoint position)
{
    QPoint diff = startingPoint - position;
    m_graphicView->horizontalScrollBar()->setValue(horizontalStart + diff.x());
    m_graphicView->verticalScrollBar()->setValue(verticalStart + diff.y());
}
void MapFrame::currentCursorChanged(QCursor* cursor)
{
    m_currentCursor = cursor;
    m_graphicView->setCursor(*m_currentCursor);
}
void MapFrame::currentToolChanged(ToolsBar::SelectableTool selectedtool)
{
    m_currentTool = selectedtool;
    if(m_map != NULL)
        m_map->setCurrentTool(m_currentTool);

}
void MapFrame::mousePressEvent(QMouseEvent* event)
{
/**
  @TODO : stop the event when we are not in normal editing mode and make appropriate actions.
  */


    if(m_currentEditingMode != ColorSelector::NORMAL)
    {
           event->ignore();
    }
    else
        SubMdiWindows::mousePressEvent(event);
}

void MapFrame::paintEvent(QPaintEvent* event)
{
    /*if(m_currentEditingMode != ColorSelector::NORMAL)
    {
        event->accept();
        
    }
    else*/
        SubMdiWindows::paintEvent(event);
}

void MapFrame::currentPenSizeChanged(int ps)
{
    if(m_map != NULL)
        m_map->setPenSize(ps);
}
void MapFrame::currentNPCSizeChanged(int ps)
{
    if(m_map != NULL)
        m_map->setNPCSize(ps);
}

void MapFrame::currentColorChanged(QColor& penColor)
{
    m_penColor = penColor;

    if(m_map !=NULL)
        m_map->setCurrentChosenColor(m_penColor);
}
void MapFrame::setEditingMode(int mode)
{
    m_currentEditingMode = mode;
}
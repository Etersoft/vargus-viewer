/*
List widget with cameras.

Copyright (C) 2012-2013 Etersoft <info@etersoft.ru>

This file is part of VargusViewer.

VargusViewer is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

VargusViewer is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#include "cameralist.h"
#include<QListIterator>
#include<QDrag>
#include<QMimeData>
#include<QMouseEvent>
#include<QApplication>
#include "videowidget/videowidgetdragdata.h"

CameraList::CameraList(QWidget *parent) :
    QListWidget(parent)
{
    setSelectionMode(QAbstractItemView::SingleSelection);
    setDropIndicatorShown(true);
    setDragEnabled(true);
}

void CameraList::print()
{
    clear();
    int camNum = 1;
    foreach(Camera *c, currentCameras)
        addItem(QString::number(camNum++) + ". " + c->description());
    foreach(Camera *c, activeCameras)
    {
        camNum = 0;
        foreach(Camera *cam, currentCameras)
        {
            if(c == cam)
            {
                item(camNum)->setBackgroundColor(QColor(0, 255, 255));
                break;
            }
            camNum++;
        }
    }
}

void CameraList::startDrag()
{
    QDrag* drag = new QDrag(this);
    // The data to be transferred by the drag and drop operation is contained in a QMimeData object
    QMimeData *data = new QMimeData;
    data->setText(QString("varguscamera") + QString::number(QCoreApplication::applicationPid()));
    int r = currentRow();
    if( r < 0 || r >= currentCameras.length())
        return;
    Camera * c = currentCameras.at(r);
    data->setUserData(0, (QObjectUserData*) new VideoWidgetDragData(c));
    //Assign ownership of the QMimeData object to the QDrag object.
    drag->setMimeData(data);
    // Start the drag and drop operation
    drag->start();
}

void CameraList::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
        startPos = event->pos();
    QListWidget::mousePressEvent(event);
}

void CameraList::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        int distance = (event->pos() - startPos).manhattanLength();
        if(distance >= QApplication::startDragDistance())
            startDrag();
    }
    QListWidget::mouseMoveEvent(event);
}

void CameraList::updateCameraData(Camera *c)
{

    foreach(Camera *cam, currentCameras)
    {
        if(cam->name() == c->name())
        {
            cam->setSource(c->source());
            cam->setPreview(c->preview());
        }
    }
}

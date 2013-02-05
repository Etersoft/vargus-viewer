/*

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

#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>
#include <QDebug>
#include "videowidget.h"
#include "../logger.h"
extern Logger &vargusLog;

VideoWidget::VideoWidget(): played(FALSE)
{
    camera = NULL;
    isRunningStringActive = true;
    statusClick = NO_CLICK;
    setFrameStyle(QFrame::Panel | QFrame::Sunken);
    setAcceptDrops(true);
    setupContextMenu();
    waitingDoubleClickTimer = new QTimer();
}

VideoWidget::~VideoWidget()
{
    stopPlay();
}

void VideoWidget::setCamera(Camera *_camera)
{
    camera = _camera;
}

void VideoWidget::startPlay(sizeVideo size)
{
    if(!camera)
    {
        setPrintType(BLANK);
        update();
        return;
    }
    setlog("Start play " + camera->name() + ":" + camera->description() + QString().sprintf("%08p", this));
    played = TRUE;
    switch(size)
    {
        case BIGVIDEO:
            setStretchType(CROP);
            setVideo(camera->source());
            break;
        case SMALLVIDEO:
            setStretchType(FILL);
            setVideo(camera->preview());
            break;
    }
    start();
}

void VideoWidget::stopPlay()
{
    setlog("stopPlay play " + QString().sprintf("%08p", this));
    played = FALSE;
    stop();
}

QString VideoWidget::getPaintText()
{
    if(camera)
        return camera->runningText->getLimitLine();
    else
        return QString("");
}

QString VideoWidget::getTitle()
{
    if(camera)
        return camera->name() + ":" + camera->description();
    else
        return QString("");
}

bool VideoWidget::isNeedPaintText()
{
    if(camera)
    {
        return (isRunningStringActive && camera->isRunningStringActive);
    }
    else
    {
        return isRunningStringActive;
    }
}

void VideoWidget::dragMoveEvent(QDragMoveEvent *de)
{
    de->accept();
}

void VideoWidget::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void VideoWidget::dragLeaveEvent ( QDragLeaveEvent * )
{
    //empty event
}

void VideoWidget::setupContextMenu()
{
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    contextMenu = new QMenu(this);
    arhiveCallAction = new QAction(this);
    arhiveCallAction->setText(tr("Archive"));
    changeStateMessageWidgetAction = new QAction(this);

    changeStateMessageCameraAction = new QAction(this);

    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(ContextMenuAction(const QPoint& z)));
}

void VideoWidget::contextMenuAction(const QPoint& z)
{
    if(isRunningStringActive)
        changeStateMessageWidgetAction->setText(tr("Disable message on widget"));
    else
        changeStateMessageWidgetAction->setText(tr("Enable message on widget"));

    if(camera->isRunningStringActive)
        changeStateMessageCameraAction->setText(tr("Disable message on camera"));
    else
        changeStateMessageCameraAction->setText(tr("Enable message on camera"));

    contextMenu->clear();
    disconnect(arhiveCallAction, 0, 0, 0);
    disconnect(changeStateMessageWidgetAction, 0, 0, 0);
    disconnect(changeStateMessageCameraAction, 0, 0, 0);

    contextMenu->addAction(arhiveCallAction);
    contextMenu->addAction(changeStateMessageWidgetAction);
    contextMenu->addAction(changeStateMessageCameraAction);

    connect(arhiveCallAction, SIGNAL(triggered()), this,SLOT(arhiveMenuPress()));
    connect(changeStateMessageWidgetAction, SIGNAL(triggered()), this, SLOT(changeStateMessageWidgetPress()));
    connect(changeStateMessageCameraAction, SIGNAL(triggered()), this, SLOT(changeStateMessageCameraPress()));
    contextMenu->exec(mapToGlobal(z));
}

void VideoWidget::arhiveMenuPress()
{
    setlog("press call arhive " + QString().sprintf("%08p", this));
    if(camera)
    {
        emit arhiveCall(camera->name());
    }
}

void VideoWidget::changeStateMessageWidgetPress()
{
    isRunningStringActive = !isRunningStringActive;
}

void VideoWidget::changeStateMessageCameraPress()
{
    if(camera)
    {
        camera->isRunningStringActive = !camera->isRunningStringActive;
    }
}

void VideoWidget::mousePressEvent ( QMouseEvent * e )
{
     if(e->button() == Qt::RightButton)
    {
        contextMenuAction(e->pos());
    }

    if(e->button() != Qt::LeftButton)
        return;

    statusClick =  CLICK;

    connect(waitingDoubleClickTimer, SIGNAL(timeout()), this, SLOT(waitingDoubleClickTimeout()));
    waitingDoubleClickTimer->start(QApplication::doubleClickInterval());
}

void VideoWidget::mouseReleaseEvent (QMouseEvent *)
{
    statusClick =  NO_CLICK;
}

void VideoWidget::startDrag()
{
    setlog("start drag " + QString().sprintf("%08p", this));
    if(camera == NULL)
        return;
    QDrag* drag = new QDrag(this);
    // The data to be transferred by the drag and drop operation is contained in a QMimeData object
    setlog("Drag proc " + QString::number(QCoreApplication::applicationPid()));

    QMimeData *data = new QMimeData;
    data->setText(QString("varguscamera") + QString::number(QCoreApplication::applicationPid()));
    data->setUserData(0, (QObjectUserData*)camera);
    data->setUserData(1, (QObjectUserData*)this);

    // Assign ownership of the QMimeData object to the QDrag object.
    drag->setMimeData(data);
    // Start the drag and drop operation
    drag->start();
}

void VideoWidget::mouseDoubleClickEvent ( QMouseEvent * )
{
    statusClick = DOUBLE_CLICK;
    emit bigSizeCall(this);
}

void VideoWidget::waitingDoubleClickTimeout()
{
    waitingDoubleClickTimer->stop();
    if(statusClick ==  CLICK)
    {
        statusClick = NO_CLICK;
        startDrag();
    }
}

void VideoWidget::dropEvent(QDropEvent *de)
{
   setlog("drop " + QString().sprintf("%08p", this));
   VideoWidget* dragVideoWindet;
   bool fromAnotherWidget = false;

   if(!de->mimeData()->hasText())
       return;
   QString checktext = de->mimeData()->text();
   if(checktext.compare(QString("varguscamera") + QString::number(QCoreApplication::applicationPid())))
       return;
   setlog("Drop proc " +  QString::number(QCoreApplication::applicationPid()));

   dragVideoWindet = (VideoWidget*)de->mimeData()->userData(1);
   Camera *dragCamera;
   dragCamera = (Camera*)de->mimeData()->userData(0);
   if(dragVideoWindet)
   {
       if(this == dragVideoWindet)
           return;
        dragVideoWindet->stopPlay();
        dragVideoWindet->setCamera(camera);
        dragVideoWindet->startPlay(SMALLVIDEO);
        fromAnotherWidget = true;
   }
   stopPlay();
   camera = dragCamera;
   startPlay(SMALLVIDEO);
   emit camerasChanged(this, dragCamera, fromAnotherWidget);
}

void VideoWidget::printString()
{
}

bool VideoWidget::isPlaying()
{
    return played;
}

void VideoWidget::setlog(QString log)
{
    vargusLog.writeToFile(log);
}

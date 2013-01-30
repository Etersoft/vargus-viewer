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

#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H
#include <QMenu>
#include "vlccontrol.h"
#include "../camera.h"

class VideoWidget: public VlcControl, public PrintRunningString
{
    Q_OBJECT
public:
    VideoWidget();
    ~VideoWidget();

private:
    Camera *camera;

    QMenu* contextMenu;
    QAction *arhiveCallAction;
    QAction *changeStateMessageCameraAction;
    QAction *changeStateMessageWidgetAction;
    void setupContextMenu();

    enum statusesClick{CLICK, DOUBLE_CLICK, NO_CLICK};
    statusesClick statusClick;
    QTimer *waitingDoubleClickTimer;

    bool isRunningStringActive;

    void printString();

    bool played;
public:
    void setCamera(Camera *_camera);

    enum sizeVideo {BIGVIDEO, SMALLVIDEO};
    void startPlay(sizeVideo size);
    void stopPlay();

    bool isPlaying();

    void disableTextString();
    Camera * getCamera() { return camera; }


protected:
    void mousePressEvent ( QMouseEvent * e );
    void mouseReleaseEvent (QMouseEvent * e);
    void mouseDoubleClickEvent ( QMouseEvent * event );

    void dropEvent(QDropEvent *de);
    void dragMoveEvent(QDragMoveEvent *de);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragLeaveEvent ( QDragLeaveEvent * event );
    void startDrag();

    virtual bool isneedPrintTextEvents();

    virtual QString getPaintText();
    virtual QString getTitle();

public slots:
    void contextMenuAction(const QPoint& z);
    void arhiveMenuPress();
    void changeStateMessageWidgetPress();
    void changeStateMessageCameraPress();

    void waitingDoubleClickTimeout();

signals:
    void arhiveCall(QString cam);
    void disconnectedSignal(VideoWidget *);
    void bigSizeCall(VideoWidget *);
    void camerasChanged(VideoWidget *,Camera *second, bool fromAnotherWidget);
};

#endif // VIDEOWIDGET_H

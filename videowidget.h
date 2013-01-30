/*
Used for showing video.

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

#include <QWidget>
#include <QUrl>
#include <QDrag>
#include <QMenu>

#include <vlc/vlc.h>
#include "camera.h"
#include "runningstring/runningstring.h"
#include "videowidgetlowlevelpainting.h"
#include <QVBoxLayout>
#include <QLabel>

class QFrame;

class VideoWidget: public VideoWidgetLowLevelPainting, public PrintRunningString
{
    Q_OBJECT
    QFrame *frame;

    QTimer *poller;
    QTimer *waitingDoubleClickTimer;
    bool isPlaying;
    bool isStatusNoSignal;

    static libvlc_instance_t *vlcInstance;
    libvlc_media_player_t *vlcPlayer;
    libvlc_media_t *vlcMedia;

    Camera *camera;

    QMenu* contextMenu;
    QAction *arhiveCallAction;
    QAction *changeStateMessageCameraAction;
    QAction *changeStateMessageWidgetAction;
    void setupContextMenu();

    enum statusesClick{CLICK, DOUBLE_CLICK, NO_CLICK};
    statusesClick StatusClick;

public:
    VideoWidget();
    ~VideoWidget();
    static void staticDestructor();

    void setCamera(Camera *_camera);

    enum sizeVideo {BIGVIDEO, SMALLVIDEO};
    void startPlay(sizeVideo size);
    void stopPlay();
    void disableTextString();
    Camera * getCamera() { return camera; }
    bool playing() { return isPlaying; }

    static void setVlcArgs( const char *const *_vlcArgs, int _numberVlcArgs);

private:
    static void clearVlc(libvlc_media_player_t *vlcPlayer, libvlc_media_t *vlcMedia);
    void printString();

    virtual void drawImage();
    virtual libvlc_media_player_t * getvlcPlayer();
    virtual void startvlcPlayer();
    virtual void stoptvlcPlayer();

    bool isRunningStringActive;

    void disconnectAction();

    static int numVlcArgs;
    static const char **VlcArgs;

    void vlcSetEvent();
    void vlcSetOffEvent();

    bool isStillPlay;
    QVBoxLayout* layout;
    QLabel* statusLabel;

    void setNosignalMessage();
    void setOffNosignalMessage();

    int afterStart;

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
    virtual QString getTextEvent();

public slots:
    void updateInterface();
    void ContextMenuAction(const QPoint& z);
    void arhiveMenuPress();
    void changeStateMessageWidgetPress();
    void changeStateMessageCameraPress();

    void waitingDoubleClickTimeout();
    float getFPS() { return libvlc_media_player_get_fps(vlcPlayer);
                /*libvlc_media_player_get_time(vlcPlayer);*/}
    void setStillPlay();

signals:
    void arhiveCall(QString cam);
    void disconnectedSignal(VideoWidget *);
    void bigSizeCall(VideoWidget *);
    void camerasChanged(VideoWidget *,Camera *second, bool fromAnotherWidget);
};

#endif // VIDEOVIDGET_H

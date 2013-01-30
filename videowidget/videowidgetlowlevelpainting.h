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

#ifndef VIDEOWIDGETLOWLEVELPAINTING_H
#define VIDEOWIDGETLOWLEVELPAINTING_H

#include <QWidget>
#include <QMutex>
#include <QPainter>
#include <QTimer>
#include <vlc/vlc.h>
#include "painttextproperties.h"

class VideoWidgetLowLevelPainting: public QWidget
{
    Q_OBJECT

public:
    VideoWidgetLowLevelPainting();
    ~VideoWidgetLowLevelPainting();

    uchar* getFrameBits();
    void mutexLock();
    void mutexUnlock();

    virtual void drawImage() = 0;

    void activateLowLevelPainting();
    void deactivateLowLevelPainting();

    void setAspectComply(bool value);
    int getXDisplacement();
    int getYDisplacement();

    void paintNoSignal();
    void paintNothing();
    void paintblank();
    static void setTextProperties(PaintTextProperties* _textProperties);

private:
    QMutex mutex;
    QImage *frame;

    QPainter painter;
    void paintEvent(QPaintEvent*);

    QTimer *repaintTimer;
    static const int repaintTime;

    int videosourceheight;
    int videosourcewidth;

    int widgetheight;
    int widgetwidth;

    int videoheight;
    int videowidth;

    int xdisplacement;
    int ydisplacement;

    float getAspectRatio();
    bool aspectComply;

    enum PaintType{NOTHING, VIDEO, NOSIGNAL, BLANK};
    PaintType type;

    bool isActiveLowLevelPainting;

    void checkChangeVideoSize();
    void printVideoFrame();
    void printNoSignal();
    void printblank();

    virtual libvlc_media_player_t *getvlcPlayer() = 0;
    virtual void startvlcPlayer() = 0;
    virtual void stoptvlcPlayer() = 0;

    static PaintTextProperties* textProperties;

protected:
    QString printedTitle;
    void restartPaintVideo();
    void updateGeometry();
    virtual bool isneedPrintTextEvents() = 0;
    virtual QString getTextEvent() = 0;

public slots:
    void animate();
};

#endif // VIDEOWIDGETLOWLEVELPAINTING_H

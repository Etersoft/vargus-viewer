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

#ifndef VLCPAINTING_H
#define VLCPAINTING_H
#include <QWidget>
#include <QFrame>
#include <QPainter>
#include <QTimer>
#include "vlccallbacks.h"
#include "paintassistant.h"

class VlcPainting:
#ifndef TESTCALLBACKPAINT
    public PaintAssistant,
#else
    public QFrame,
#endif
    public VlcCallbacks
{
    Q_OBJECT
public:
    VlcPainting();
    ~VlcPainting();

    virtual void queryUpdate();
    void printVideoFrame();

protected:
    QPainter painter;
private:
    void paintEvent(QPaintEvent*);

    QTimer* checkWidgetSizeTimer;
    static const int checkScreenTime;
    virtual QPainter* getPainter();

    void checkScreenSize();
    void checkPaintingStatus();

    int countupdated;
    void fixUpdated();
    bool isUpdated();

protected:
    void startCheckScreen();
    void stopCheckScreen();

    virtual void paintVideoFrame();

    virtual void restart() = 0;

    virtual int getXAssistDisplacement();
    virtual int getYAssistDisplacement();
public slots:
    void checkScreen();
};

#endif // VLCPAINTING_H

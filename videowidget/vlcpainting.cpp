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

#include <QDebug>
#include <QDateTime>
#include "vlcpainting.h"


//const int VlcPainting::checkScreenTime = 2000;
const int VlcPainting::checkScreenTime = 50;
const int VlcPainting::numberValidFramesLost = 20;

VlcPainting::VlcPainting():countupdated(0)
{
    setAttribute(Qt::WA_WState_InPaintEvent,false);
    checkWidgetSizeTimer = new QTimer(this);
    connect(checkWidgetSizeTimer, SIGNAL(timeout()), this, SLOT(checkScreen()));
}

VlcPainting::~VlcPainting()
{
    delete checkWidgetSizeTimer;
}

void VlcPainting::printVideoFrame()
{
    painter.begin(this);
    paintVideoFrame();
    painter.end();
}

void VlcPainting::paintVideoFrame()
{
    calllock();
    if(frame)
    {
        painter.drawImage(QPoint(getXDisplacement(), getYDisplacement()), *frame);
    }
    callunlock();
}

void VlcPainting::paintEvent(QPaintEvent*)
{
#ifdef TESTCALLBACKPAINT
    printVideoFrame();
#else
    printSelected();
#endif
}

void VlcPainting::queryUpdate()
{
    setPrintType(VIDEO);
    fixUpdated();
    //update();
    //repaint();
}

void VlcPainting::fixUpdated()
{
    if(countupdated>1000)
        countupdated=0;
    countupdated++;
}

bool VlcPainting::isUpdated()
{
    if(countupdated>0)
    {
        countupdated = 0;
        return true;
    }
    else
    {
        return false;
    }
}

void VlcPainting::startCheckScreen()
{
    checkWidgetSizeTimer->start(checkScreenTime);
}

void VlcPainting::stopCheckScreen()
{
    checkWidgetSizeTimer->stop();
}

void VlcPainting::checkScreen()
{
    checkPaintingStatus();
    checkScreenSize();
}

void VlcPainting::checkPaintingStatus()
{
    static int numberFramesLost = 0;

    if(!isUpdated() )
    {
        numberFramesLost++;
#ifndef TESTCALLBACKPAINT
        if(numberFramesLost>numberValidFramesLost)
        {
            setPrintType(NOSIGNAL);
        }
        update();
#endif
    }
    else
    {
        numberFramesLost = 0;
    }
    update();

}

void VlcPainting::checkScreenSize()
{
    if( getScreenHeight() != this->height() || getScreenWidth() != this->width())
    {
        restart();
    }
}

QPainter* VlcPainting::getPainter()
{
    return &painter;
}

int VlcPainting::getXAssistDisplacement()
{
#ifdef TESTCALLBACKPAINT
    return 0;
#else
    return getXDisplacement();
#endif
}

int VlcPainting::getYAssistDisplacement()
{
#ifdef TESTCALLBACKPAINT
    return 0;
#else
    return getYDisplacement();
#endif
}

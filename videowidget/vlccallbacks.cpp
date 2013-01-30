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
#include "vlccallbacks.h"

static void display(void *core, void *picture)
{
    Q_UNUSED(picture);
    VlcCallbacks *parrent = (VlcCallbacks *)core;
    parrent->calldisplay();
    //Q_ASSERT(picture == NULL);
}

static void *lock(void *core, void **planes)
{
    VlcCallbacks *parrent = (VlcCallbacks *)core;
    parrent->calllock();
    *planes = parrent->getFrameBits();
    return NULL;
}

static void unlock(void *core, void *picture, void *const *planes)
{
    Q_UNUSED(planes);
    Q_UNUSED(picture);

    VlcCallbacks *parrent = (VlcCallbacks *)core;
    parrent->callunlock();
    //Q_ASSERT(picture == NULL);
}


unsigned format(void **core, char *chroma, unsigned *width, unsigned *height, unsigned *pitches, unsigned *lines)
{
    VlcCallbacks *parrent = (VlcCallbacks *)*core;
    parrent->callSetupQImage((int*)width, (int*)height);
    //strcpy_s(chroma, 5, "RV32");
    strcpy(chroma, "RV32");

    (*pitches) = (*width) * 4;
    (*lines)   = (*height);
    return 1;
}

void cleanup (void *core)
{
    VlcCallbacks *parrent = (VlcCallbacks *)core;
    parrent->callReleaseQImage();
}

VlcCallbacks::VlcCallbacks():frame(NULL)
{   
    frame = NULL;
}

VlcCallbacks::~VlcCallbacks()
{
    delete frame;
}

void VlcCallbacks::calllock()
{
    mutex.lock();
}

void VlcCallbacks::callunlock()
{
    mutex.unlock();
}

void VlcCallbacks::calldisplay()
{
    queryUpdate();
}

uchar* VlcCallbacks::getFrameBits()
{
    if(frame)
    {
        return frame->bits();
    }
    else
    {
        return NULL;
    }
}

void VlcCallbacks::callSetupQImage(int *_videowidth, int *_videoheight)
{
    setVideoInformation(getScreenWidth(), getScreenHeight(),*_videowidth, *_videoheight);

    *_videowidth = getShowedWidth();
    *_videoheight = getShowedHeight();
    frame = new QImage(getShowedWidth() , getShowedHeight()+1 , QImage::Format_ARGB32_Premultiplied);
    frame->fill(Qt::black);
}

void VlcCallbacks::callReleaseQImage()
{
    delete frame;
    frame =0;
}

void VlcCallbacks::setCallbacks(int _screenwidth, int _screenheight)
{
    setScreenSize(_screenwidth, _screenheight);
    mutex.lock();

    //libvlc_video_set_format(getvlcPlayer(), "RV32", screenwidth, screenheight, screenwidth * 4);
    libvlc_video_set_callbacks(getvlcPlayer(), lock, unlock, display, this);
    libvlc_video_set_format_callbacks(getvlcPlayer(), format , cleanup);
    mutex.unlock();
}

void VlcCallbacks::cleanCallbacks()
{
    libvlc_video_set_format_callbacks(getvlcPlayer(), NULL, NULL);
    libvlc_video_set_callbacks(getvlcPlayer(), NULL, NULL, NULL, this);
}




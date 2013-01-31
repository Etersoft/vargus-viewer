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

#ifndef VLCCALLBACKS_H
#define VLCCALLBACKS_H

#include <QMutex>
#include <QImage>
#include "vlc/vlc.h"
#include "vlc/libvlc_structures.h"
#include "videomath.h"

class VlcCallbacks : public VideoMath
{
public:
    VlcCallbacks();
    ~VlcCallbacks();

    void calllock();
    void callunlock();
    void calldisplay();
    void callSetupQImage(int* _videowidth, int* _videoheight);
    void callReleaseQImage();
    void callLog(int level, const char *fmt, va_list args);

    uchar* getFrameBits();

    virtual libvlc_media_player_t *getvlcPlayer() = 0;

    #ifdef newversionofvlc
        static libvlc_log_subscriber_t debug_subscriber;
    #endif

    void setCallbacks(int _screenwidth, int _screenheight);
    void cleanCallbacks();

    void setStaticCallbacks();
private:
    QMutex mutex;

protected:
    virtual void queryUpdate() = 0;
    QImage* frame;

    virtual void setlog(QString log) = 0;
};

#endif // VLCCALLBACKS_H

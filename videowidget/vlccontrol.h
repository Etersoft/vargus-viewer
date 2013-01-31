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

#ifndef VLCCONTROL_H
#define VLCCONTROL_H
#include <QString>
#include "vlcpainting.h"

class VlcControl : public VlcPainting
{
public:
    VlcControl();
    ~VlcControl();

    static void setVlcArgs( const char *const *_vlcArgs, int _numberVlcArgs);

    void setVideo (QString source);
    void start();
    void stop();

    static void staticDestructor();

    virtual bool isPlaying();
protected:
    virtual void restart();

    virtual void setlog(QString log);
private:
    virtual libvlc_media_player_t* getvlcPlayer();

    static const char **vlcArgs;
    static int numberVlcArgs;

    static libvlc_instance_t *vlcInstance;
    libvlc_media_player_t *vlcPlayer;
    libvlc_media_t *vlcMedia;
};

#endif // VLCCONTROL_H

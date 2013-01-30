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

#include "vlccontrol.h"

libvlc_instance_t *VlcControl::vlcInstance = 0;
const char **VlcControl::vlcArgs = 0;
int VlcControl::numberVlcArgs = 0;

VlcControl::VlcControl()
{
    if(!vlcInstance)
    {
        const char * const vlc_args[] = {
                  "-I", "dummy", /* Don't use any interface */
                  "--ignore-config", /* Don't use VLC's config */
                  "--verbose=2"//be much more verbose then normal for debugging purpose,
                    };
        vlcInstance = libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);
    }
    vlcPlayer = libvlc_media_player_new(vlcInstance);
}

void VlcControl::staticDestructor()
{
    libvlc_release(vlcInstance);
}

void VlcControl::setVlcArgs( const char *const *_vlcArgs, int _numberVlcArgs)
{
    vlcArgs = (const char **)_vlcArgs;
    numberVlcArgs = _numberVlcArgs;
}

VlcControl::~VlcControl()
{
    stop();
    libvlc_media_player_release(vlcPlayer);
    libvlc_media_release(vlcMedia);
}

void VlcControl::setVideo(QString source)
{
    vlcMedia = libvlc_media_new_path(vlcInstance, source.toAscii());
    libvlc_media_player_set_media (vlcPlayer, vlcMedia);
}

void VlcControl::start()
{
    libvlc_media_player_play(vlcPlayer);
    while(libvlc_media_player_play(vlcPlayer))
    {

    }
#ifndef TESTCALLBACKPAINT
    setPrintType(VIDEO);
#endif
    setCallbacks(this->width(), this->height());
    startCheckScreen();
}

void VlcControl::stop()
{
#ifndef TESTCALLBACKPAINT
    setPrintType(NOTHING);
#endif
    stopCheckScreen();
    libvlc_media_player_stop(vlcPlayer);
    while(libvlc_media_player_play(vlcPlayer))
    {

    }
    cleanCallbacks();
}

void VlcControl::restart()
{
    stop();
    start();
}

libvlc_media_player_t* VlcControl::getvlcPlayer()
{
    return vlcPlayer;
}

bool VlcControl::isPlaying()
{
    if(libvlc_media_player_is_playing(vlcPlayer))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}



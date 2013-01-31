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

VlcControl::VlcControl(): vlcMedia(0)
{
    setlog("VlcControl constructor " + QString().sprintf("%08p", this));
    if(!vlcInstance)
    {
        for(int i = 0; i < numberVlcArgs; i++ )
        {
            if(strcmp(vlcArgs[i], "--video-title-show") == 0)
            {
                vlcArgs[i] = "--no-video-title-show";
            }
        }
        vlcInstance = libvlc_new(numberVlcArgs, vlcArgs);
        setStaticCallbacks();
    }
    if(!vlcInstance)
    {
        setlog("ERROR Create vlc instance. May be needs libvlc.dll" + QString().sprintf("%08p", this));
        exit(0);
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
    setlog("VlcControl destructor " + QString().sprintf("%08p", this));
    stop();
    libvlc_media_player_release(vlcPlayer);
    if(vlcMedia)
    {
        libvlc_media_release(vlcMedia);
    }
}

void VlcControl::setVideo(QString source)
{
    setlog("set media " + source);
    if(vlcMedia)
    {
        libvlc_media_release(vlcMedia);
    }
    vlcMedia = libvlc_media_new_path(vlcInstance, source.toAscii());
    libvlc_media_player_set_media (vlcPlayer, vlcMedia);
}

void VlcControl::start()
{
    setlog("start " + QString().sprintf("%08p", this));
    libvlc_media_player_play(vlcPlayer);
#ifndef TESTCALLBACKPAINT
    setPrintType(VIDEO);
#endif
    setCallbacks(this->width(), this->height());
    startCheckScreen();
}

void VlcControl::stop()
{
    setlog("stop " + QString().sprintf("%08p", this));
#ifndef TESTCALLBACKPAINT
    setPrintType(NOTHING);
#endif
    stopCheckScreen();
    libvlc_media_player_stop(vlcPlayer);
    cleanCallbacks();
}

void VlcControl::restart()
{
    setlog("restart " + QString().sprintf("%08p", this));
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

void VlcControl::setlog(QString log)
{
}

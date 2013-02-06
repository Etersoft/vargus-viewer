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
#include "videomath.h"

VideoMath::VideoMath():stretchType(FILL), xDisplacement(0),yDisplacement(0),aspectRatio(1.0), screenwidth(0)
{
}

uint VideoMath::getScreenWidth()
{
    return screenwidth;
}

uint VideoMath::getScreenHeight()
{
    return screenheight;
}

uint VideoMath::getVideoWidth()
{
    return videowidth;
}

uint VideoMath::getVideoHeight()
{
    return videoheight;
}

void VideoMath::setStretchType(StretchType _stretchType)
{
    stretchType = _stretchType;
}

int VideoMath::getYDisplacement()
{
    return yDisplacement;
}

int VideoMath::getXDisplacement()
{
    return xDisplacement;
}

float VideoMath::getAspectRatio()
{
    return aspectRatio;
}

void VideoMath::setScreenSize(uint _screenwidth,uint _screenheight)
{
    screenwidth = _screenwidth;
    screenheight = _screenheight;
}

void VideoMath::setVideoInformation(uint _screenwidth, uint _screenheight, uint _videowidth, uint _videoheight)
{
    screenwidth = _screenwidth;
    screenheight = _screenheight;
    videowidth = _videowidth;
    videoheight = _videoheight;

    calcAspectRatio();
    calcShowedSize();
    calcYDisplacement();
    calcXDisplacement();

}

void VideoMath::calcXDisplacement()
{
    if(videowidth <= 0 || videoheight <= 0 || screenheight <= 0 || screenwidth <= 0)
    {
        xDisplacement = 0;
    }

    switch(stretchType)
    {
    case FILL:
        xDisplacement = 0;
        break;
    case CROP:
        xDisplacement = screenwidth > showedwidth?((screenwidth - showedwidth)/2):0;
        break;
    }
}

void VideoMath::calcYDisplacement()
{
    if(videowidth <= 0 || videoheight <= 0 || screenheight <= 0 || screenwidth <= 0)
    {
        yDisplacement = 0;
    }

    switch(stretchType)
    {
    case FILL:
        yDisplacement = 0;
        break;
    case CROP:
        yDisplacement = screenheight > showedheight?((screenheight - showedheight)/2):0;
        break;
    }
}

void VideoMath::calcAspectRatio()
{
    if(videowidth <= 0 || videoheight <= 0 )
    {
        aspectRatio = 1.0;
    }

    switch(stretchType)
    {
    case FILL:
        aspectRatio = 1.0;
        break;
    case CROP:
        aspectRatio = (float)videowidth/(float)videoheight;;
        break;
    }
}

void VideoMath::calcShowedSize()
{
    switch(stretchType)
    {
    case FILL:
        showedwidth = screenwidth;
        showedheight = screenheight;
        break;
    case CROP:
        if(((float)screenwidth/(float)screenheight) > getAspectRatio() )
        {
            showedwidth = (uint)(screenheight * getAspectRatio());
            showedheight = screenheight;
        }
        else
        {
            showedwidth = screenwidth;
            showedheight = (uint)(screenwidth/getAspectRatio());
        }
        break;
    }
}

uint VideoMath::getShowedWidth()
{
    return showedwidth;
}

uint VideoMath::getShowedHeight()
{
    return showedheight;
}

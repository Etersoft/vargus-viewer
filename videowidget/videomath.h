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

#ifndef VIDEOMATH_H
#define VIDEOMATH_H

typedef unsigned int uint;

class VideoMath
{    
public:
    VideoMath();

    enum StretchType {FILL, CROP};
private:
    StretchType stretchType;
    int xDisplacement;
    int yDisplacement;

    float aspectRatio;

    uint screenwidth;
    uint screenheight;

    uint videowidth;
    uint videoheight;

    uint showedwidth;
    uint showedheight;

    void calcXDisplacement();
    void calcYDisplacement();
    void calcAspectRatio();
    void calcShowedSize();
protected:
    uint getScreenWidth();
    uint getScreenHeight();

    uint getVideoWidth();
    uint getVideoHeight();

    uint getShowedWidth();
    uint getShowedHeight();

    void setStretchType(StretchType _stretchType);
    int getYDisplacement();
    int getXDisplacement();
    float getAspectRatio();

    void setScreenSize(uint _screenwidth, uint _screenheight);
    void setVideoInformation(uint _screenwidth, uint _screenheight, uint _videowidth, uint _videoheight);
};

#endif // VIDEOMATH_H

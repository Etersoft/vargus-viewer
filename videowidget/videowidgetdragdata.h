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

#ifndef VIDEOWIDGETDRAGDATA_H
#define VIDEOWIDGETDRAGDATA_H

#include <QObjectUserData>
#include "videowidget.h"
#include "../camera.h"

class VideoWidgetDragData : public QObjectUserData
{
public:
    VideoWidgetDragData(Camera *_camera, VideoWidget *_videowidget);
    VideoWidgetDragData(Camera *_camera);
    ~VideoWidgetDragData();

    Camera *getCamera();
    VideoWidget *getVideoWidget();

    enum TypeDrag { DRAG_FROM_VIDEO_WIDGET, DRAG_FROM_LIST};
    TypeDrag getTypeDrag ();
private:
    Camera *camera;
    VideoWidget *videowidget;
    TypeDrag typeDrag;
};

#endif // VIDEOWIDGETDRAGDATA_H

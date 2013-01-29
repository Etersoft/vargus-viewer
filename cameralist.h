/*
List widget with cameras.

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

#ifndef CAMERALIST_H
#define CAMERALIST_H

#include <QListWidget>
#include "camera.h"
#include<QList>

class CameraList : public QListWidget
{
    Q_OBJECT

    QList<Camera *> currentCameras;
    QList<Camera *> activeCameras;
    QPoint startPos;

public:
    explicit CameraList(QWidget *parent = 0);
    void setActiveCameras(const QList<Camera *> &cams) { activeCameras = cams; }
    void setCurrentCameras(const QList<Camera *> &cams) { currentCameras = cams; }
    void print();
    Camera * getCamera(int num) { return currentCameras.at(num); }
    void updateCameraData(Camera *c);

protected:
    void startDrag();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);    
};

#endif // CAMERALIST_H

/*
Store information about camera.

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

#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include "runningstring/runningstring.h"
#include "runningstring/limitline.h"

class Camera : public QObject, public AppendRunningString
{
    Q_OBJECT

    QString cam_name;
    QString cam_description;
    QString cam_source;
    QString cam_preview;
    QString cam_agent;
    int cameraNum;

public:
    explicit Camera(const QString &cam);
    ~Camera();
    Camera * copy();

    QString name() {return cam_name;}
    QString description() {return cam_description;}
    QString source() {return cam_source;}
    QString preview() {return cam_preview;}
    QString agent() {return cam_agent;}
    int number() {return cameraNum;}

    void setName(const QString &new_value);
    void setDescription(const QString &new_value){cam_description = new_value;}
    void setSource(const QString &new_value){cam_source = new_value;}
    void setPreview(const QString &new_value){cam_preview = new_value;}
    void setAgent(const QString &new_value){cam_agent = new_value;}
    void setNumber(int _newNumber){cameraNum= _newNumber;}

    static RunningTextSettings* runningTextSetting;
    static RunningString* runningString;
    LimitLine* runningText;
    bool isRunningStringActive;

private:
    static QString runningTextip;
    static int runningTextport;

    void setStaticParam();

public:
    static void setRunningTextAddress(QString ip, int port);
    static void changeTextServerSettings(const QString &_adress, int _port);

    virtual void appendString(QString str);
};

#endif // CAMERA_H

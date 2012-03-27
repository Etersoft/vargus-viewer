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

public:
    explicit CameraList(QWidget *parent = 0);
    void setActiveCameras(QList<Camera *> cams) { activeCameras = cams; }
    void setCurrentCameras(QList<Camera *> cams) { currentCameras = cams; }
    void print();
signals:

protected:
    void startDrag();
    
public slots:
    
};

#endif // CAMERALIST_H

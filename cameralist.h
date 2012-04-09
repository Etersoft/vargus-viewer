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
signals:

protected:
    void startDrag();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    
public slots:
    
};

#endif // CAMERALIST_H

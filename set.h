#ifndef SET_H
#define SET_H

#include <QObject>
#include "camera.h"

class Set : public QObject
{
    Q_OBJECT

    QString set_description;
    QList<Camera *> cameraList;

public:
    explicit Set();
    Set(QString desc){set_description = desc;}

    QString description(){return set_description;}
    void addCamera(Camera* cam);
    QStringList camerasNames();
    QList<Camera *> cameras(){return cameraList;}

signals:

public slots:

};

#endif // SET_H

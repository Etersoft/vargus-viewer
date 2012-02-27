#ifndef SET_H
#define SET_H

#include <QObject>
#include "camera.h"

class Set : public QObject
{
    Q_OBJECT

    QString description;
    QList<Camera *> cameraList;

public:
    explicit Set(QObject *parent = 0);

signals:

public slots:

};

#endif // SET_H

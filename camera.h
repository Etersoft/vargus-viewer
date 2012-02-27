#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>

class Camera : public QObject
{
    Q_OBJECT

    QString name;
    QString description;
    QString source;
    QString preview;
    QString agent;

public:
    explicit Camera(QObject *parent = 0);

signals:

public slots:

};

#endif // CAMERA_H

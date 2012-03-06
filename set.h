#ifndef SET_H
#define SET_H

#include <QWidget>
#include "camera.h"
#include "view.h"

class Set : public QWidget
{
    Q_OBJECT

    QString set_description;
    QList<Camera *> cameraList;
    QList<View *> viewList;

public:
    explicit Set();
    Set(QString desc){set_description = desc;}

    QString description(){return set_description;}
    void addView(View* view);
    void addCamera(Camera* cam);
    QStringList camerasNames();
    QList<Camera *> cameras(){return cameraList;}
    QList<View *> views(){return viewList;}

signals:

public slots:

};

#endif // SET_H

#include "set.h"
#include <QStringList>

Set::Set()
{
}

void Set::addCamera(Camera* cam)
{
    cameraList << cam;
}

QStringList Set::camerasNames()
{
    QStringList list;
    for(int i = 0; i < cameraList.count(); i++)
        list << cameraList.at(i)->name();
    return list;
}

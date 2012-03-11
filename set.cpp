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

void Set::addView(View *view)
{
    View *new_view = new View(view);
    connect(new_view, SIGNAL(waitForUpdate()), this, SLOT(updateActiveView()));
    viewList << new_view;
}

void Set::setActiveView(int index)
{
    for(int i = viewList.count() - 1; i >= 0; i--)
        viewList.at(index)->setActive(i == index);
}

void Set::updateActiveView()
{
    for(int i = 0; i < viewList.count(); i++)
    {
        viewList.at(i)->updateActivity();
    }
}

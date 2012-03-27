#include "cameralist.h"
#include<QListIterator>
#include<QDrag>
#include<QMimeData>


CameraList::CameraList(QWidget *parent) :
    QListWidget(parent)
{
    setSelectionMode(QAbstractItemView::SingleSelection);
    setDropIndicatorShown(true);
    setDragEnabled(true);

}

void CameraList::print()
{
    QList<Camera *>::iterator it = currentCameras.begin();
    QList<Camera *>::iterator end = currentCameras.end();
    clear();
    int camNum =1;
    while(it != end)
    {
        addItem(QString::number(camNum++) + ". " + (*it)->description());
        it++;
    }
    QList<Camera *>::iterator itac = activeCameras.begin();
    QList<Camera *>::iterator endac = activeCameras.end();
    while(itac != endac)
    {
        int cameraNum = 0;
        it = currentCameras.begin();
        while(it != end)
        {
            if((*itac) == (*it))
            {
                QColor color(0,255,255);
                item(cameraNum)->setBackgroundColor(color);
                break;
            }
            it++;
            cameraNum++;
        }
        itac++;
    }
}

void CameraList::startDrag()
{
    QDrag* drag = new QDrag(this);
    // The data to be transferred by the drag and drop operation is contained in a QMimeData object
    QMimeData *data = new QMimeData;
    data->setUserData(0,(QObjectUserData*)currentCameras.at(currentRow()));
    //Assign ownership of the QMimeData object to the QDrag object.
    drag->setMimeData(data);
    // Start the drag and drop operation
    drag->start();
}

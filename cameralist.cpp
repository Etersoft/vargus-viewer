#include "cameralist.h"
#include<QListIterator>
#include<QDrag>
#include<QMimeData>
#include<QMouseEvent>
#include<QApplication>


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
    int r = currentRow();
    if( r < 0 || r >= currentCameras.length())
        return;
    Camera * c = currentCameras.at(r);
    data->setUserData(0,(QObjectUserData*)c);
    //Assign ownership of the QMimeData object to the QDrag object.
    drag->setMimeData(data);
    // Start the drag and drop operation
    drag->start();
}

void CameraList::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
        startPos=event->pos();
    QListWidget::mousePressEvent(event);
}

void CameraList::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        int distance = (event->pos() - startPos).manhattanLength();
        if(distance >= QApplication::startDragDistance())
            startDrag();
    }
    QListWidget::mouseMoveEvent(event);
}

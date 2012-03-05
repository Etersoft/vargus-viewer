#include "view.h"

View::View()
{
}

void View::setDoubleFrames(QStringList list)
{
    while(!list.isEmpty())
        view_double << list.takeAt(0).toInt();
}

void View::setTripleFrames(QStringList list)
{
    while(!list.isEmpty())
        view_triple << list.takeAt(0).toInt();
}

void View::setQuadrupleFrames(QStringList list)
{
    while(!list.isEmpty())
        view_quadruple << list.takeAt(0).toInt();
}

void View::createIcon()
{
    QPixmap image(ICON_WIDTH, ICON_HEIGHT);
    image.fill();


    view_icon = new QIcon(image);
}


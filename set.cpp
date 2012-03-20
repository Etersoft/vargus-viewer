#include "set.h"
#include <QStringList>
#include<QGridLayout>

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
    setLayouts(index);
}

void Set::updateActiveView()
{
    int type = 0;
    for(int i = 0; i < viewList.count(); i++)
    {
        if(viewList.at(i)->updateActivity())
            type = i;
    }
    setLayouts(type);

}

void Set::makeTwoSquare()
{
    QGridLayout *grid  = new QGridLayout(this);
    setLayout(grid);
    QPushButton *b;
    b = new QPushButton(this);
    videoList << b;
    grid->addWidget(b,0,0);
    b = new QPushButton(this);
    videoList << b;
    grid->addWidget(b,0,1);
    b = new QPushButton(this);
    videoList << b;
    grid->addWidget(b,1,0);
    b = new QPushButton(this);
    videoList << b;
    grid->addWidget(b,1,1);
}

void Set::makeFourSquareTripple()
{
    QGridLayout *grid  = new QGridLayout(this);
    setLayout(grid);
    for(int i = 0; i < 6; i++)
        videoList << new QPushButton(this);
    grid->addWidget(videoList.at(0),0,0,2,2);
    grid->addWidget(videoList.at(1),0,2);
    grid->addWidget(videoList.at(2),1,2);
    grid->addWidget(videoList.at(3),2,0);
    grid->addWidget(videoList.at(4),2,1);
    grid->addWidget(videoList.at(5),2,2);
}

void Set::makeFourSquareOneCentral()
{
    QGridLayout *grid  = new QGridLayout(this);
    setLayout(grid);
    for(int i = 0; i < 13; i++)
        videoList << new QPushButton(this);
    grid->addWidget(videoList.at(0),0,0);
    grid->addWidget(videoList.at(1),0,1);
    grid->addWidget(videoList.at(2),0,2);
    grid->addWidget(videoList.at(3),0,3);
    grid->addWidget(videoList.at(4),1,0);
    grid->addWidget(videoList.at(5),1,1,2,2);
    grid->addWidget(videoList.at(6),1,3);
    grid->addWidget(videoList.at(7),2,0);
    grid->addWidget(videoList.at(8),2,3);
    grid->addWidget(videoList.at(9),3,0);
    grid->addWidget(videoList.at(10),3,1);
    grid->addWidget(videoList.at(11),3,2);
    grid->addWidget(videoList.at(12),3,3);
}

void Set::makeFiveSquareTwoOneTripple()
{
    QGridLayout *grid  = new QGridLayout(this);
    setLayout(grid);
    for(int i = 0; i < 11; i++)
        videoList << new QPushButton(this);
    grid->addWidget(videoList.at(0),0,0,3,3);
    grid->addWidget(videoList.at(1),0,3,2,2);
    grid->addWidget(videoList.at(2),2,3,2,2);
    grid->addWidget(videoList.at(3),3,0);
    grid->addWidget(videoList.at(4),3,1);
    grid->addWidget(videoList.at(5),3,2);
    for(int i = 0; i < 5; i++)
        grid->addWidget(videoList.at(i+6),4,i);
}

void Set::makeFourSquare()
{
    QGridLayout *grid = new QGridLayout(this);
    setLayout(grid);
    for(int i = 0; i < 48; i++)
        videoList << new QPushButton(this);
    int y = 0;
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 6; j++)
        {
            grid->addWidget(videoList.at(y++),j,i);
        }
    }
}

void Set::makeTrippleSquare()
{
    QGridLayout *grid  = new QGridLayout(this);
    setLayout(grid);
    for(int i = 0; i < 21; i++)
        videoList << new QPushButton(this);
    for(int i = 0; i < 6; i++)
        grid->addWidget(videoList.at(i),0,i);
    for(int i = 0; i < 6; i++)
        grid->addWidget(videoList.at(i+15),5,i);
    grid->addWidget(videoList.at(6),1,0);
    grid->addWidget(videoList.at(7),1,1,4,4);
    grid->addWidget(videoList.at(8),1,5);
    grid->addWidget(videoList.at(9),2,0);
    grid->addWidget(videoList.at(10),2,5);
    grid->addWidget(videoList.at(11),3,0);
    grid->addWidget(videoList.at(12),3,5);
    grid->addWidget(videoList.at(13),4,0);
    grid->addWidget(videoList.at(14),4,5);

}

void Set::setLayouts(int type)
{
    if(layout()!= NULL)
        delete layout();
    for(int i = 0; i < videoList.length(); i++)
        delete videoList.at(i);
    videoList.clear();

    switch(type)
    {
        case(0):
        {
            makeTwoSquare();
            break;
        }
        case(1):
        {
            makeFourSquareTripple();
            break;
        }
        case(2):
        {
            makeFourSquareOneCentral();
            break;
        }
        case(3):
        {
            makeFiveSquareTwoOneTripple();
            break;
        }
       case(4):
        {
            makeTrippleSquare();
            break;
        }
        case(5):
        {
            makeFourSquare();
            break;
        }

     }

}

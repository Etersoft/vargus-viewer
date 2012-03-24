#include "set.h"
#include <QStringList>
#include<QGridLayout>
#include<QListIterator>


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
    QList<View *>::iterator it = viewList.begin();
    QList<View *>::iterator end = viewList.end();
    int n = 0;
    while(it!=end)
    {
        (*it)->setActive( n == index);
        it++;
        n++;
    }
}

void Set::updateActiveView()
{
    int type = 0;
    QList<View *>::iterator it = viewList.begin();
    QList<View *>::iterator end = viewList.end();
    int n = 0;
    while(it!=end)
    {
        if((*it)->updateActivity())
            type = n;
        it++;
        n++;

    }
    setLayouts(type);
}

void Set::makeTwoSquare()
{
    QGridLayout *grid = new QGridLayout(this);
    setLayout(grid);
    int y = 0;
    bool f = true;
    for( int i =0; i < 2 && f; i++)
    {
        for( int j = 0; j < 2; j++)
        {
            if(videoList.length() == y)
            {
                f = false;
                break;
            }
            grid->addWidget(videoList.at(y),i,j);
            videoList.at(y)->startPlay(VideoWidget::SMALLVIDEO);
            videoList.at(y++)->show();
        }
    }
}

void Set::makeFourSquareTripple()
{
    QGridLayout *grid = new QGridLayout(this);
    setLayout(grid);

    if(videoList.length() < 1) return;
    grid->addWidget(videoList.at(0),0,0,2,2);
    videoList.at(0)->startPlay(VideoWidget::SMALLVIDEO);
    videoList.at(0)->show();

    if(videoList.length() < 2) return;
    grid->addWidget(videoList.at(1),0,2);
    videoList.at(1)->startPlay(VideoWidget::SMALLVIDEO);
    videoList.at(1)->show();

    if(videoList.length() < 3) return;
    grid->addWidget(videoList.at(2),1,2);
    videoList.at(2)->startPlay(VideoWidget::SMALLVIDEO);
    videoList.at(2)->show();

    for(int i = 0; i < 3; i++)
    {
        if(videoList.length()-3 + i < 1)
            return;
        grid->addWidget(videoList.at(3+i),2,i);
        videoList.at(3+i)->startPlay(VideoWidget::SMALLVIDEO);
        videoList.at(3+i)->show();
    }




}

void Set::makeFourSquareOneCentral()
{
    QGridLayout *grid = new QGridLayout(this);
    setLayout(grid);
    for(int i = 0; i < 4; i++)
    {
        if( videoList.length() - i < 1 ) return;
        grid->addWidget(videoList.at(i), 0, i);
        videoList.at(i)->startPlay(VideoWidget::SMALLVIDEO);
        videoList.at(i)->show();
    }
    if(videoList.length() < 5) return;
    grid->addWidget(videoList.at(4),1,0);
    videoList.at(4)->startPlay(VideoWidget::SMALLVIDEO);
    videoList.at(4)->show();

    if(videoList.length() < 6) return;
    grid->addWidget(videoList.at(5),1,1,2,2);
    videoList.at(5)->startPlay(VideoWidget::SMALLVIDEO);
    videoList.at(5)->show();

    if(videoList.length() < 7) return;
    grid->addWidget(videoList.at(6),1,3);
    videoList.at(6)->startPlay(VideoWidget::SMALLVIDEO);
    videoList.at(6)->show();

    if(videoList.length() < 8) return;
    grid->addWidget(videoList.at(7),2,0);
    videoList.at(7)->startPlay(VideoWidget::SMALLVIDEO);
    videoList.at(7)->show();

    if(videoList.length() < 9) return;
    grid->addWidget(videoList.at(8),2,3);
    videoList.at(8)->startPlay(VideoWidget::SMALLVIDEO);
    videoList.at(8)->show();

    for(int i = 0; i < 4; i++)
    {
        if( videoList.length() -9 - i  < 1 ) return;
        grid->addWidget(videoList.at(i + 9), 3, i);
        videoList.at(i + 9)->startPlay(VideoWidget::SMALLVIDEO);
        videoList.at(i+9)->show();
    }


}

void Set::makeFiveSquareTwoOneTripple()
{
    QGridLayout *grid  = new QGridLayout(this);
    setLayout(grid);
    if(videoList.length() < 1) return;
    grid->addWidget(videoList.at(0),0,0,3,3);
    videoList.at(0)->startPlay(VideoWidget::SMALLVIDEO);
    videoList.at(0)->show();

    if(videoList.length() < 2) return;
    grid->addWidget(videoList.at(1),0,3,2,2);
    videoList.at(1)->startPlay(VideoWidget::SMALLVIDEO);
    videoList.at(1)->show();

    if(videoList.length() < 3) return;
    grid->addWidget(videoList.at(2),2,3,2,2);
    videoList.at(2)->startPlay(VideoWidget::SMALLVIDEO);
    videoList.at(2)->show();

    for( int i = 0; i < 3; i++)
    {
        if(videoList.length() - 3 - i < 1) return;
        grid->addWidget(videoList.at(i+3),3,i);
        videoList.at(i+3)->startPlay(VideoWidget::SMALLVIDEO);
        videoList.at(i+3)->show();
    }
    for( int i = 0; i < 5; i++)
    {
        if(videoList.length() - 6 - i < 1) return;
        grid->addWidget(videoList.at(i+6),4,i);
        videoList.at(i+6)->startPlay(VideoWidget::SMALLVIDEO);
        videoList.at(i+6)->show();
    }
}

void Set::makeFourSquare()
{
    QGridLayout *grid = new QGridLayout(this);
    setLayout(grid);
    QList<VideoWidget *>::iterator it = videoList.begin();
    QList<VideoWidget *>::iterator end = videoList.end();
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 6; j++)
        {
            if(it == end) return;
            grid->addWidget(*it,i,j);
            (*it)->startPlay(VideoWidget::SMALLVIDEO);
            (*it)->show();
            it++;
        }
    }



}

void Set::makeTrippleSquare()
{
    QGridLayout *grid  = new QGridLayout(this);
    setLayout(grid);
    QList<VideoWidget *>::iterator it = videoList.begin();
    QList<VideoWidget *>::iterator end = videoList.end();
    for(int i = 0; i < 3; i++)
        for( int j = 0; j < 3; j++)
        {
            if(it == end) return;
            grid->addWidget(*it,i,j);
            (*it)->startPlay(VideoWidget::SMALLVIDEO);
            (*it)->show();
            it++;
        }
}

void Set::setLayouts(int type)
{
    if(layout()!= NULL)
        delete layout();
    stopPlay();
    tp = type;
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

void Set::makeVideoWidgets()
{
    QList<Camera *>::iterator it = cameraList.begin();
    QList<Camera *>::iterator end = cameraList.end();
    while(it != end)
    {
        VideoWidget *v = new VideoWidget();
        v->setUrlVideoStream((*it)->preview(), VideoWidget::SMALLVIDEO);
        v->setUrlVideoStream((*it)->source(), VideoWidget::BIGVIDEO);
        videoList << v;
        it++;
    }
}

void Set::stopPlay()
{
    QList<VideoWidget *>::iterator it = videoList.begin();
    QList<VideoWidget *>::iterator end = videoList.end();
    while(it!=end)
    {
        (*it)->stopPlay();
        if((*it)->isVisible())
            (*it)->hide();
        it++;
    }
}

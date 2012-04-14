#include "set.h"
#include"logger.h"
#include <QStringList>
#include<QGridLayout>
#include<QListIterator>
extern Logger &vargusLog;
Set::Set()
{
    activeCameras = tp = 0;
    buttonClicked = active = false;
    bigPlaying = NULL;
    lastCamNum = NULL;
}

Set::Set(const QString &desc)
{
    activeCameras = tp = 0;
    set_description = desc;
    buttonClicked = active = false;
    bigPlaying = NULL;
    lastCamNum = NULL;
    wasChanged = NULL;

}
Set::~Set()
{
    QList<VideoWidget *>::iterator it = videoList.begin();
    QList<VideoWidget *>::iterator end = videoList.end();
    while(it != end)
    {
        delete (*it);
        it++;
    }
    QList<Camera *>::iterator itc = cameraList.begin();
    QList<Camera *>::iterator endc = cameraList.end();
    while(itc != endc)
    {
        delete(*itc);
        itc++;
    }
    QList<View *>::iterator itv = viewList.begin();
    QList<View *>::iterator endv = viewList.end();
    while(itv != endv)
    {
        delete (*itv);
        itv++;
    }
    for(int i = 0; i < stc.size(); i++)
    {
        delete stc[i];
    }
    if(lastCamNum != NULL)
        delete[] lastCamNum;
    if(wasChanged != NULL)
        delete[] wasChanged;
}

void Set::addCamera(Camera* cam)
{
    cameraList << cam->copy();
}

QStringList Set::camerasNames()
{
    QStringList list;
    QList<Camera *>::iterator it = cameraList.begin();
    QList<Camera *>::iterator end = cameraList.end();
    while(it != end)
    {
        list << (*it) -> name();
        it++;
    }
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
    while( it != end )
    {
        if((*it) -> updateActivity())
            type = n;
        it++;
        n++;
    }
     setLayouts(type);
}

void Set::makeTwoSquare()
{
    QGridLayout *grid = new QGridLayout(this);
    grid -> setMargin(0);
    setLayout(grid);
    for( int i = 0; i < 2; i++)
    {
        for( int j = 0; j < 2; j++)
        {
            VideoWidget *v = new VideoWidget();
            grid->addWidget(v, i, j);
            videoList << v;
        }
    }
}

void Set::makeFourSquareTripple()
{
    QGridLayout *grid = new QGridLayout(this);
    grid -> setMargin(0);
    setLayout(grid);

    VideoWidget *v = new VideoWidget();
    grid->addWidget(v, 0, 0, 2, 2);
    videoList << v;

    v = new VideoWidget();
    grid->addWidget(v, 0, 2);
    videoList << v;

    v = new VideoWidget();
    videoList << v;
    grid->addWidget(v, 1, 2);

    for(int i = 0; i < 3; i++)
    {
        v = new VideoWidget();
        videoList << v;
        grid->addWidget(v, 2, i);
    }
}

void Set::makeFourSquareOneCentral()
{
    QGridLayout *grid = new QGridLayout(this);
    grid -> setMargin(0);
    setLayout(grid);
    for(int i = 0; i < 4; i++)
    {
        VideoWidget *v = new VideoWidget();
        videoList << v;
        grid->addWidget(v, 0, i);
    }
    VideoWidget *v = new VideoWidget();
    videoList << v;
    grid->addWidget(v, 1, 0);

    v = new VideoWidget();
    videoList << v;
    grid->addWidget(v, 1, 1, 2, 2);

    v = new VideoWidget();
    videoList << v;
    grid->addWidget(v, 1, 3);

    v = new VideoWidget();
    videoList << v;
    grid->addWidget(v, 2, 0);

    v = new VideoWidget();
    videoList << v;
    grid->addWidget(v, 2, 3);


    for(int i = 0; i < 4; i++)
    {
        v = new VideoWidget();
        videoList << v;
        grid->addWidget(v, 3, i);
    }
}

void Set::makeFiveSquareTwoOneTripple()
{
    QGridLayout *grid  = new QGridLayout(this);
    grid -> setMargin(0);
    setLayout(grid);
    VideoWidget *v = new VideoWidget();
    videoList << v;
    grid->addWidget(v, 0, 0, 3, 3);

    v = new VideoWidget();
    videoList << v;
    grid->addWidget(v, 0, 3, 2, 2);

    v = new VideoWidget();
    videoList << v;
    grid->addWidget(v, 2, 3, 2, 2);

    for( int i = 0; i < 3; i++)
    {
        v = new VideoWidget();
        videoList << v;
        grid->addWidget(v, 3, i);
    }
    for( int i = 0; i < 5; i++)
    {
        v = new VideoWidget();
        videoList << v;
        grid->addWidget(v, 4, i);
    }
}

void Set::makeTrippleSquare()
{
    QGridLayout *grid  = new QGridLayout(this);
    grid -> setMargin(0);
    setLayout(grid);
    for(int i = 0; i < 3; i++)
        for( int j = 0; j < 3; j++)
        {
            VideoWidget *v = new VideoWidget();
            videoList << v;
            grid->addWidget(v, i, j);
        }
}

void Set::makeFourSquare()
{
    QGridLayout *grid = new QGridLayout(this);
    grid->setMargin(0);
    setLayout(grid);
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            VideoWidget *v = new VideoWidget();
            videoList << v;
            grid->addWidget(v, i, j);

        }
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
    countActiveAndPlay();
    emit updateActiveCameras(getActiveCameras());

}

void Set::init()
{
    lastCamNum = new int[viewList.length()];
    wasChanged = new bool[viewList.length()];
    for(int i = 0; i < viewList.length(); i++)
    {
        QList<Camera *>* tmpc = new QList<Camera *>(cameraList);
        stc << tmpc;
        lastCamNum[i] = (amountOfCells(i) < cameraList.length()) ? (amountOfCells(i) - 1) : (cameraList.length() - 1);
    }

}

void Set::stopPlay(VideoWidget *excluding)
{
    QList<VideoWidget *>::iterator it = videoList.begin();
    QList<VideoWidget *>::iterator end = videoList.end();
    while(it!=end)
    {
        if(*it != excluding)
            delete *it;
        it++;
    }
    bigPlaying = NULL;
    videoList.clear();
    if(excluding)
        videoList << excluding;
}

QList<Camera*> Set::getActiveCameras()
{
    QList<Camera *> res;
    QList<Camera *>::iterator it = stc.at(tp)->begin();
    int activeCameras = amountOfPlayingWidgets();
    for(int i = 0; i < activeCameras; i++, it++)
        res << (*it);
    return res;
}

void Set::next()
{
    QList<Camera *> *currentList = stc.at(tp);
    if(amountOfCells(tp) >= currentList -> length() || lastCamNum[tp] == cameraList.length() - 1)
            return;
    buttonClicked = true;
    currentList -> clear();
    for(int i = lastCamNum[tp]+1; i < cameraList.length(); i++)
    {
        currentList -> push_back(cameraList.at(i));
    }
    for(int i = 0; i <= lastCamNum[tp]; i++)
    {
        currentList -> push_back(cameraList.at(i));
    }
    setLayouts(tp);
    lastCamNum[tp] += activeCameras;
    if(lastCamNum[tp] >= currentList -> length())
        lastCamNum[tp] = currentList -> length() - 1;
    emit updateActiveCameras(getActiveCameras());
    buttonClicked = false;
    wasChanged[tp] = true;
}

void Set::prev()
{
    int number = amountOfCells(tp);
    if(number >= stc.at(tp)->length() || lastCamNum[tp] == activeCameras - 1)
        return;
    lastCamNum[tp] = lastCamNum[tp] - number * 2;
    if(lastCamNum[tp] < -1)
        lastCamNum[tp] = -1;
    next();
}

void Set::reset()
{
    if(!wasChanged[tp])
        return;
    QList<Camera *> *currentList = stc.at(tp);
    currentList -> clear();
    QList<Camera *>::iterator itc = cameraList.begin();
    QList<Camera *>::iterator endc = cameraList.end();
    while(itc != endc)
    {
       currentList -> push_back(*itc);
       itc++;
    }
    setLayouts(tp);
    lastCamNum[tp] = activeCameras - 1;
    wasChanged[tp] = false;
}

void Set::bigVideo(VideoWidget *v)
{
    if(v == bigPlaying)
    {
        setLayouts(tp);
        return;
    }
    QList<Camera *> res;
    res << v -> getCamera();
    v -> stopPlay();
    stopPlay(v);
    delete layout();
    QGridLayout *grid  = new QGridLayout(this);
    setLayout(grid);
    grid -> setMargin(0);
    grid -> addWidget(v,0,0);
    v -> startPlay(VideoWidget::BIGVIDEO);
    v -> show();
    bigPlaying = v;
    emit updateActiveCameras(res);
}

void Set::bigVideo(Camera *c)
{
    stopPlay();
    delete layout();
    QGridLayout *grid  = new QGridLayout(this);
    setLayout(grid);
    VideoWidget *v = new VideoWidget();
    v -> setCamera(c);
    connect(v,SIGNAL(bigSizeCall(VideoWidget*)),this,SLOT(bigVideo(VideoWidget*)));
    grid -> addWidget(v,0,0);
    v -> startPlay(VideoWidget::BIGVIDEO);
    videoList << v;
    bigPlaying = v;
    QList<Camera *> res;
    res << c;
    emit updateActiveCameras(res);
}

void Set::showBig(int num)
{
    bigVideo(cameraList.at(num));
}

void Set::countActiveAndPlay()
{
    const QList<Camera *> *currentList = stc.at(tp);
    int len = currentList -> length();
    activeCameras = (len < videoList.length()) ? len : videoList.length();
    if(buttonClicked && (activeCameras + lastCamNum[tp]) > len)
        activeCameras = len - lastCamNum[tp] - 1;
    QList<VideoWidget *>::iterator it = videoList.begin();
    for(int i =  0; i < activeCameras; i++, it++)
    {
        (*it) -> setCamera(currentList -> at(i));
        (*it) -> startPlay(VideoWidget::SMALLVIDEO);
    }
    it = videoList.begin();
    QList<VideoWidget *>::iterator end = videoList.end();
    while(it != end)
    {
        connect(*it,SIGNAL(bigSizeCall(VideoWidget*)),this,SLOT(bigVideo(VideoWidget*)));
        connect(*it,SIGNAL(camerasChanged(VideoWidget *,Camera *,bool)),this,SLOT(changeCameras(VideoWidget*,Camera*,bool)));
        it++;
    }
}

void Set::changeCameras(VideoWidget *first, Camera *second, bool fromAnotherWidget)
{
    QList<Camera *> *currentList = stc.at(tp);
    if(!fromAnotherWidget)
    {
        int f = videoList.indexOf(first);
        currentList -> removeAt(f);
        currentList -> insert(f,second);
        vargusLog.writeToFile("Change camera in widget " + QString::number(f) + " to " + second->description());
        emit updateActiveCameras(getActiveCameras());
        wasChanged[tp] = true;
        return;
    }
    QList<Camera *>::iterator it = currentList -> begin();
    QList<Camera *>::iterator end = currentList -> end();
    int f = videoList.indexOf(first);
    int s = 0;
    int i = 0;
    while(it != end)
    {
        if(*it == second)
        {
            s = i;
            break;
        }
        it++;
        i++;
    }
    vargusLog.writeToFile("Swap widgets " + QString::number(f) + " " + QString::number(s));
    if(f >= currentList -> length())
    {
        int k = f - currentList -> length() + 1;
        for(int i = 0; i < k; i++)
            currentList -> push_back(NULL);
        lastCamNum[tp] += k;
    }
    else if(it == end)
    {
        currentList -> push_back(NULL);
        lastCamNum[tp]++;
        s = currentList -> length() - 1;
    }
    currentList -> swap(f, s);
    wasChanged[tp] = true;
    emit updateActiveCameras(getActiveCameras());
}

int Set::amountOfCells(int tp)
{
    switch(tp)
    {
        case(0):
        {
            return 4;
        }
        case(1):
        {
            return 6;
        }
        case(2):
        {
            return 13;
        }
        case(3):
        {
            return 11;
        }
       case(4):
        {
            return 9;
        }
        case(5):
        {
            return 16;
        }
    }
    return 0;
}

int Set::amountOfPlayingWidgets()
{
    QList<VideoWidget *>::iterator it = videoList.begin();
    QList<VideoWidget *>::iterator end = videoList.end();
    int k = 0;
    while( it != end )
    {
        if( (*it) -> playing() )
            k++;
        it++;
    }
    return k;
}

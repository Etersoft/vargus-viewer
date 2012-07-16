#include "set.h"
#include"logger.h"
#include <QStringList>
#include<QGridLayout>
#include<QListIterator>
#include"mainwindow.h"
extern Logger &vargusLog;
Set::Set(MainWindow *_mainwindow)
{
    tp = 0;
    active = false;
    bigPlaying = NULL;
    lastCamNum = NULL;
    mainwindow = _mainwindow;

}

Set::Set(const QString &desc, MainWindow *_mainwindow)
{
    tp = 0;
    set_description = desc;
    active = false;
    bigPlaying = NULL;
    lastCamNum = NULL;
    offset = NULL;
    wasChanged = NULL;
    mainwindow = _mainwindow;
}

Set::~Set()
{
    vargusLog.writeToFile(QString("Destructor of set %1 started").arg(description()));
   /* QList<VideoWidget *>::iterator it = videoList.begin();
    QList<VideoWidget *>::iterator end = videoList.end();
    while(it != end)
    {
        (*it)->hide();
        it++;
    }*/
    //stopPlay();
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
    if(offset != NULL)
        delete[] offset;
    vargusLog.writeToFile(QString("Destructor of set %1 ended").arg(description()));
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
    while(it != end)
    {
        (*it) -> setActive( n == index);
        it++;
        n++;
    }
    tp = index;
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
     enableButtons();
}

void Set::setLayouts(int type)
{
    if(layout()!= NULL)
        delete layout();
    stopPlay();
    tp = type;


    QGridLayout *grid = new QGridLayout(this);
    grid -> setMargin(0);

    QList<int> doubleCells = viewList.at(tp)->doubleCells();
    QList<int> tripleCells = viewList.at(tp)->tripleCells();
    QList<int> quadrupleCells = viewList.at(tp)->quadrupleCells();
    int viewWidth = viewList.at(tp)->width();
    int viewHeight = viewList.at(tp)->height();
    bool **net = new bool*[viewHeight];//не забыть удалить
    for(int i = 0; i < viewHeight; i++)
    {
        bool *tmp = new bool[viewWidth];
        for(int j = 0; j < viewWidth; j++)
            tmp[j] = true;//свободный
        net[i] = tmp;
    }

    for(int i = 0; i < viewHeight; i++)
    {
        for(int j = 0; j < viewWidth;)
        {
            while((net[i][j] == false) && (j < viewWidth))
                j++;
            if(j == viewWidth)
                break;
            VideoWidget *v = new VideoWidget();
            videoList << v;
            int cellNum = (j+1) + viewWidth*i;
            if(doubleCells.contains(cellNum))
            {
                grid -> addWidget(v,i,j,2,2);
                net[i][j] = false;
                net[i][j+1] = false;
                net[i+1][j] = false;
                net[i+1][j+1] = false;
                j+=2;
            }
            else if(tripleCells.contains(cellNum))
            {
                grid -> addWidget(v,i,j,3,3);
                net[i][j] = false;
                net[i][j+1] = false;
                net[i][j+2] = false;
                net[i+1][j] = false;
                net[i+1][j+1] = false;
                net[i+1][j+2] = false;
                net[i+2][j] = false;
                net[i+2][j+1] = false;
                net[i+2][j+2] = false;
                j+=3;
            }
            else if(quadrupleCells.contains(cellNum))
            {
                grid -> addWidget(v,i,j,4,4);
                for(int k = i; k <= i+4; k++)
                    for(int m = j; m <= j+4; m++)
                        net[k][m] =false;
                j+=4;
            }
            else
            {
                grid->addWidget(v,i,j);
                net[i][j] = false;
                j++;
            }
        }
    }
    for(int i = 0; i < viewHeight; i++)
    {
        delete[] net[i];
    }
    delete[] net;
    countActiveAndPlay();
    emit updateActiveCameras(getActiveCameras());

}

void Set::init(VPlayingType t, Container *_videoContainer)
{
    vargusLog.writeToFile("Init set started");
    videoContainer = _videoContainer;
    lastCamNum = new int[viewList.length()];
    wasChanged = new bool[viewList.length()];
    offset = new int[viewList.length()];
    for(int i = 0; i < viewList.length(); i++)
    {
        QList<Camera *> * tmpc = new QList<Camera *>(cameraList);
        stc << tmpc;
        lastCamNum[i] = (amountOfCells(i) < cameraList.length()) ? (amountOfCells(i) - 1) : (cameraList.length() - 1);
        offset[i] = 0;
    }
    VideoWidget::setVPlayingType(t);
}

void Set::stopPlay(VideoWidget *excluding)
{
    vargusLog.writeToFile(QString("Stop play %1").arg(description()));
    QList<VideoWidget *>::iterator it = videoList.begin();
    QList<VideoWidget *>::iterator end = videoList.end();
    QList<VideoWidget *> widgetsToDelete;
    while(it != end)
    {
        if(*it != excluding)
        {
            (*it)->hide();
            widgetsToDelete.push_back(*it);
        }
        it++;
    }
    if(widgetsToDelete.isEmpty() == false)
        videoContainer->addVideoWidgets(widgetsToDelete);
    bigPlaying = NULL;
    videoList.clear();
    if(excluding)
        videoList << excluding;
    vargusLog.writeToFile(QString("Stop play %1 return").arg(description()));
}

QList<Camera*> Set::getActiveCameras()
{
    QList<Camera *> res;
    QList<VideoWidget *>::iterator it = videoList.begin();
    QList<VideoWidget *>::iterator end = videoList.end();
    while(it != end)
    {
        if( (*it) -> playing() )
        {
            res << (*it) -> getCamera();
        }
        it++;
    }
    return res;
}

void Set::next()
{
    QList<Camera *> *currentList = stc.at(tp);
    currentList -> clear();
    int newFirstCam = offset[tp]*amountOfCells(tp)+lastCamNum[tp] + 1;
    lastCamNum[tp] = cameraList.length() - (newFirstCam + 1);
    if(lastCamNum[tp] >= amountOfCells(tp))
    {
        lastCamNum[tp] = amountOfCells(tp) - 1;
    }
    else if(amountOfCells(tp) > lastCamNum[tp]+1)
    {
        newFirstCam = newFirstCam - amountOfCells(tp) + lastCamNum[tp] +1 ;
        lastCamNum[tp] = amountOfCells(tp)-1;
    }
    for(int i = newFirstCam; i < cameraList.length(); i++)
    {
        currentList -> push_back(cameraList.at(i));
    }
    for(int i = 0; i < newFirstCam; i++)
    {
        currentList -> push_back(cameraList.at(i));
    }
    if(lastCamNum[tp] >= amountOfCells(tp))
        lastCamNum[tp] = amountOfCells(tp) - 1;
    offset[tp]++;
    setLayouts(tp);
    wasChanged[tp] = true;
    enableButtons();
}

void Set::prev()
{
    QList<Camera *> *currentList = stc.at(tp);
    currentList -> clear();
    offset[tp]--;
    lastCamNum[tp] = amountOfCells(tp) - 1;
    int newFirstCam = offset[tp]*amountOfCells(tp);
    for(int i = newFirstCam; i < cameraList.length(); i++)
    {
        currentList -> push_back(cameraList.at(i));
    }
    for(int i = 0; i < newFirstCam; i++)
    {
        currentList -> push_back(cameraList.at(i));
    }
    wasChanged[tp] = true;
    setLayouts(tp);
    enableButtons();
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
    offset[tp] = 0;
    int lastCam = amountOfCells(tp) - 1;
    if(lastCam >= cameraList.length())
        lastCam = cameraList.length();
    lastCamNum[tp] = lastCam;
    setLayouts(tp);
    wasChanged[tp] = false;
    enableButtons();
}

void Set::bigVideo(VideoWidget *v)
{
    wasChanged[tp] = true;
    if(v == bigPlaying)
    {
        setLayouts(tp);
        bigPlaying = NULL;
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
    wasChanged[tp] = true;
    stopPlay();
    delete layout();
    QGridLayout *grid  = new QGridLayout(this);
    setLayout(grid);
    VideoWidget *v = new VideoWidget();
    v -> setCamera(c);
    connect(v,SIGNAL(bigSizeCall(VideoWidget*)),this,SLOT(bigVideo(VideoWidget*)));
    connect(v, SIGNAL(disconnectedSignal(VideoWidget*)), this, SLOT(restoreVideoWidget(VideoWidget*)));
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
    QList<VideoWidget *>::iterator it = videoList.begin();
    for(int i =  0; i <= lastCamNum[tp]; i++, it++)
    {
        (*it) -> setCamera(currentList -> at(i));
        (*it) -> startPlay(VideoWidget::SMALLVIDEO);
    }
    it = videoList.begin();
    QList<VideoWidget *>::iterator end = videoList.end();
    while(it != end)
    {
        connect(*it,SIGNAL(bigSizeCall(VideoWidget*)),this,SLOT(bigVideo(VideoWidget*)));
        connect(*it, SIGNAL(camerasChanged(VideoWidget *, Camera *, bool)),
                this, SLOT(changeCameras(VideoWidget*, Camera*, bool)));
        connect(*it, SIGNAL(disconnectedSignal(VideoWidget*)), this, SLOT(restoreVideoWidget(VideoWidget*)));
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
        vargusLog.writeToFile("Change camera in widget "
                              + QString::number(f) + " to " + second -> description());
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

    QList<int> doubleCells = viewList.at(tp)->doubleCells();
    QList<int> tripleCells = viewList.at(tp)->tripleCells();
    QList<int> quadrupleCells = viewList.at(tp)->quadrupleCells();
    int viewWidth = viewList.at(tp)->width();
    int viewHeight = viewList.at(tp)->height();
    int k = viewWidth * viewHeight;
    k = k - doubleCells.length()*3;
    k = k - tripleCells.length() * 8;
    k = k - quadrupleCells.length() * 15;
    return k;
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

void Set::enableButtons()
{
    int newFirstNextCam = offset[tp]*amountOfCells(tp)+lastCamNum[tp] + 1;
    bool nextButton = true;
    if(newFirstNextCam >= cameraList.length())
        nextButton = false;
    bool prevButton = true;
    if(offset[tp] == 0)
        prevButton = false;
    emit buttonsEnabled(prevButton, nextButton);
}

bool Set::setPlayingType(VPlayingType t)
{
    QList<VideoWidget *>::iterator it = videoList.begin();
    QList<VideoWidget *>::iterator end = videoList.end();
    while(it != end)
    {
        if(((*it) -> playing()))
        {
            (*it) -> stopPlay();
            if(bigPlaying)
                (*it) -> startPlay(VideoWidget::BIGVIDEO);
            else
                (*it) -> startPlay(VideoWidget::SMALLVIDEO);
        }
        it++;
    }
    VideoWidget::setVPlayingType(t);
    return true;
}

void Set::restoreVideoWidget(VideoWidget *v)
{
    vargusLog.writeToFile("Try to restore videowidget");
    Camera *c = v->getCamera();
    mainwindow->updateCamera(c);
    if(bigPlaying == v)
        v->startPlay(VideoWidget::BIGVIDEO);
    else
        v->startPlay(VideoWidget::SMALLVIDEO);

}


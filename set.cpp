#include "set.h"
#include <QStringList>
#include<QGridLayout>
#include<QListIterator>

Set::Set(QString desc)
{
    activeCameras = tp = 0;
    set_description = desc;
    active = false;
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
    for(int i = 0; i < st.size(); i++)
    {
        delete st[i];
    }
    for(int i = 0; i < stc.size(); i++)
    {
        delete stc[i];
    }
}

void Set::addCamera(Camera* cam)
{

    cameraList << cam->copy();
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
    QList<VideoWidget *>::const_iterator it = ((st.at(0)))->begin();
    QList<VideoWidget *>::const_iterator end = (st.at(0))->end();
    for( int i =0; i < 2; i++)
    {
        for( int j = 0; j < 2; j++)
        {
            if(it == end) return;
            grid->addWidget(*it,i,j);
            (*it)->startPlay(VideoWidget::SMALLVIDEO);
            (*it)->show();
            it++;
            activeCameras++;
        }
    }
}

void Set::makeFourSquareTripple()
{
    QGridLayout *grid = new QGridLayout(this);
    setLayout(grid);

    if(st.at(1)->length() < 1) return;
    grid->addWidget(st.at(1)->at(0),0,0,2,2);
    st.at(1)->at(0)->startPlay(VideoWidget::SMALLVIDEO);
    st.at(1)->at(0)->show();
    activeCameras++;

    if(st.at(1)->length() < 2) return;
    grid->addWidget(st.at(1)->at(1),0,2);
    st.at(1)->at(1)->startPlay(VideoWidget::SMALLVIDEO);
    st.at(1)->at(1)->show();
    activeCameras++;

    if(st.at(1)->length() < 3) return;
    grid->addWidget(st.at(1)->at(2),1,2);
    st.at(1)->at(2)->startPlay(VideoWidget::SMALLVIDEO);
    st.at(1)->at(2)->show();
    activeCameras++;

    for(int i = 0; i < 3; i++)
    {
        if(st.at(1)->length()-3 + i < 1)
            return;
        grid->addWidget(st.at(1)->at(3+i),2,i);
        st.at(1)->at(3+i)->startPlay(VideoWidget::SMALLVIDEO);
        st.at(1)->at(3+i)->show();
        activeCameras++;
    }
}

void Set::makeFourSquareOneCentral()
{
    QGridLayout *grid = new QGridLayout(this);
    setLayout(grid);
    for(int i = 0; i < 4; i++)
    {
        if( st.at(2)->length() - i < 1 ) return;
        grid->addWidget(st.at(2)->at(i), 0, i);
        st.at(2)->at(i)->startPlay(VideoWidget::SMALLVIDEO);
        st.at(2)->at(i)->show();
        activeCameras++;
    }
    if(st.at(1)->length() < 5) return;
    grid->addWidget(st.at(2)->at(4),1,0);
    st.at(2)->at(4)->startPlay(VideoWidget::SMALLVIDEO);
    st.at(2)->at(4)->show();
    activeCameras++;

    if(st.at(2)->length() < 6) return;
    grid->addWidget(st.at(2)->at(5),1,1,2,2);
    st.at(2)->at(5)->startPlay(VideoWidget::SMALLVIDEO);
    st.at(2)->at(5)->show();
    activeCameras++;

    if(st.at(2)->length() < 7) return;
    grid->addWidget(st.at(2)->at(6),1,3);
    st.at(2)->at(6)->startPlay(VideoWidget::SMALLVIDEO);
    st.at(2)->at(6)->show();
    activeCameras++;

    if(st.at(2)->length() < 8) return;
    grid->addWidget(st.at(2)->at(7),2,0);
    st.at(2)->at(7)->startPlay(VideoWidget::SMALLVIDEO);
    st.at(2)->at(7)->show();
    activeCameras++;

    if(st.at(2)->length() < 9) return;
    grid->addWidget(st.at(2)->at(8),2,3);
    st.at(2)->at(8)->startPlay(VideoWidget::SMALLVIDEO);
    st.at(2)->at(8)->show();
    activeCameras++;

    for(int i = 0; i < 4; i++)
    {
        if( st.at(2)->length() -9 - i  < 1 ) return;
        grid->addWidget(st.at(2)->at(i + 9), 3, i);
        st.at(2)->at(i + 9)->startPlay(VideoWidget::SMALLVIDEO);
        st.at(2)->at(i+9)->show();
        activeCameras++;
    }
}

void Set::makeFiveSquareTwoOneTripple()
{
    QGridLayout *grid  = new QGridLayout(this);
    setLayout(grid);
    if(st.at(3)->length() < 1) return;
    grid->addWidget(st.at(3)->at(0),0,0,3,3);
    st.at(3)->at(0)->startPlay(VideoWidget::SMALLVIDEO);
    st.at(3)->at(0)->show();
    activeCameras++;

    if(st.at(3)->length() < 2) return;
    grid->addWidget(st.at(3)->at(1),0,3,2,2);
    st.at(3)->at(1)->startPlay(VideoWidget::SMALLVIDEO);
    st.at(3)->at(1)->show();
    activeCameras++;

    if(st.at(3)->length() < 3) return;
    grid->addWidget(st.at(3)->at(2),2,3,2,2);
    st.at(3)->at(2)->startPlay(VideoWidget::SMALLVIDEO);
    st.at(3)->at(2)->show();
    activeCameras++;

    for( int i = 0; i < 3; i++)
    {
        if(st.at(3)->length() - 3 - i < 1) return;
        grid->addWidget(st.at(3)->at(i+3),3,i);
        st.at(3)->at(i+3)->startPlay(VideoWidget::SMALLVIDEO);
        st.at(3)->at(i+3)->show();
        activeCameras++;
    }
    for( int i = 0; i < 5; i++)
    {
        if(st.at(3)->length() - 6 - i < 1) return;
        grid->addWidget(st.at(3)->at(i+6),4,i);
        st.at(3)->at(i+6)->startPlay(VideoWidget::SMALLVIDEO);
        st.at(3)->at(i+6)->show();
        activeCameras++;
    }
}

void Set::makeTrippleSquare()
{
    QGridLayout *grid  = new QGridLayout(this);
    setLayout(grid);
    QList<VideoWidget *>::const_iterator it = st.at(4)->begin();
    QList<VideoWidget *>::const_iterator end = st.at(4)->end();
    for(int i = 0; i < 3; i++)
        for( int j = 0; j < 3; j++)
        {
            if(it == end) return;
            grid->addWidget(*it,i,j);
            (*it)->startPlay(VideoWidget::SMALLVIDEO);
            (*it)->show();
            it++;
            activeCameras++;
        }
}

void Set::makeFourSquare()
{
    QGridLayout *grid = new QGridLayout(this);
    setLayout(grid);
    QList<VideoWidget *>::const_iterator it = st.at(5)->begin();
    QList<VideoWidget *>::const_iterator end = st.at(5)->end();
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 6; j++)
        {
            if(it == end) return;
            grid->addWidget(*it,i,j);
            (*it)->startPlay(VideoWidget::SMALLVIDEO);
            (*it)->show();
            it++;
            activeCameras++;
        }
    }
}

void Set::setLayouts(int type)
{
    if(layout()!= NULL)
        delete layout();
    stopPlay();
    tp = type;
    activeCameras = 0;
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
    emit updateActiveCameras(getActiveCameras());

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
        connect(v,SIGNAL(bigSizeCall(VideoWidget*)),this,SLOT(bigVideo(VideoWidget*)));
        connect(v,SIGNAL(arhiveCall()),this,SLOT(restoreState()));
    }
    for(int i = 0; i < viewList.length(); i++)
    {
        QList<VideoWidget *> *tmp = new QList<VideoWidget*>(videoList);
        QList<Camera *>* tmpc = new QList<Camera *>(cameraList);
        st << tmp;
        stc << tmpc;
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

QList<Camera*> Set::getActiveCameras()
{
    QList<Camera *> res;
    QList<Camera *>::iterator it = stc.at(tp)->begin();
    for(int i = 0; i < activeCameras; i++,it++)
        res << (*it);
    return res;
}

void Set::next()
{
    if(activeCameras >= st.at(tp)->length())
        return;
    for(int i = 0; i < activeCameras; i++)
    {

        st.at(tp)->move(0,st.at(tp)->length()-1);
        stc.at(tp)->move(0,stc.at(tp)->length()-1);
    }
    setLayouts(tp);
    emit updateActiveCameras(getActiveCameras());
}

void Set::prev()
{
    if(activeCameras >= st.at(tp)->length())
        return;
    for(int i = 0; i < activeCameras; i++)
    {

        st.at(tp)->move(st.at(tp)->length()-1,0);
        stc.at(tp)->move(stc.at(tp)->length()-1,0);
    }
    setLayouts(tp);
    emit updateActiveCameras(getActiveCameras());

}

void Set::reset()
{
    st.at(tp)->clear();
    stc.at(tp)->clear();
    QList<VideoWidget *>::iterator it = videoList.begin();
    QList<VideoWidget *>::iterator end = videoList.end();
    QList<Camera *>::iterator itc = cameraList.begin();
    while(it != end)
    {
       st.at(tp)->push_back(*it);
       stc.at(tp)->push_back(*itc);
       it++;
       itc++;
    }
    setLayouts(tp);
}

void Set::bigVideo(VideoWidget *v)
{
    stopPlay();
    delete layout();
    QGridLayout *grid  = new QGridLayout(this);
    setLayout(grid);
    grid->addWidget(v,0,0);
    v->startPlay(VideoWidget::BIGVIDEO);

}

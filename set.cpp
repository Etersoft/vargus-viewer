/*
Store video widgets and layouts.

Copyright (C) 2012-2013 Etersoft <info@etersoft.ru>

This file is part of VargusViewer.

VargusViewer is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

VargusViewer is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#include "set.h"
#include "logger.h"
#include <QStringList>
#include <QGridLayout>
#include <QListIterator>
#include <QDesktopServices>
#include "mainwindow.h"

extern Logger &vargusLog;

Set::Set(const QString &desc, MainWindow *_mainwindow, QString _serverAddress)
{
    tp = 0;
    set_description = desc;
    active = false;
    bigPlaying = NULL;
    lastCamNum = NULL;
    offset = NULL;
    wasChanged = NULL;
    mainwindow = _mainwindow;
    serverAddress = _serverAddress;
}

Set::~Set()
{
    vargusLog.writeToFile(QString("Destructor of set %1 started").arg(description()));
    stopPlay();
    foreach(Camera *c, cameraList)
        delete c;
    foreach(View *v, viewList)
        delete v;
    for(int i = 0; i < stc.size(); i++)
    {
        delete stc[i];
    }
    delete[] lastCamNum;
    delete[] wasChanged;
    delete[] offset;
    vargusLog.writeToFile(QString("Destructor of set %1 ended").arg(description()));
}

//add copy(!) of camera to set
void Set::addCamera(Camera* cam)
{
    cameraList << cam->copy();
}

//get names of cameras in set
QStringList Set::camerasNames()
{
    QStringList list;
    foreach(Camera *c, cameraList)
        list << c->name();
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
    int n = 0;
    foreach(View *v, viewList)
    {
        v->setActive(n == index);
        n++;
    }
    tp = index;
}

void Set::updateActiveView()
{
    int type = 0;
    int n = 0;
    foreach(View *v, viewList)
    {
        if(v->updateActivity())
            type = n;
        n++;
    }
    setLayouts(type);
    enableButtons();
}

//primary function. Videowidgets are made here
void Set::setLayouts(int type)
{
    if(layout()!= NULL)
        delete layout();
    stopPlay();
    tp = type;

    QGridLayout *grid = new QGridLayout(this);
    grid->setMargin(0);

    QList<int> doubleCells = viewList.at(tp)->doubleCells();
    QList<int> tripleCells = viewList.at(tp)->tripleCells();
    QList<int> quadrupleCells = viewList.at(tp)->quadrupleCells();
    int viewWidth = viewList.at(tp)->width();
    int viewHeight = viewList.at(tp)->height();
    bool **net = new bool*[viewHeight];
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
                grid->addWidget(v,i,j,2,2);
                net[i][j] = false;
                net[i][j+1] = false;
                net[i+1][j] = false;
                net[i+1][j+1] = false;
                j+=2;
            }
            else if(tripleCells.contains(cellNum))
            {
                grid->addWidget(v,i,j,3,3);
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
                grid->addWidget(v,i,j,4,4);
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

void Set::init()
{
    vargusLog.writeToFile("Init set started");
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
}

void Set::stopPlay(VideoWidget *excluding)
{
    vargusLog.writeToFile(QString("Stop play %1").arg(description()));
    foreach(VideoWidget *vw, videoList)
    {
        if(vw != excluding)
        {
            disconnect(vw, SIGNAL(disconnectedSignal(VideoWidget*)), this, SLOT(restoreVideoWidget(VideoWidget*)));
            delete vw;
        }
    }
    bigPlaying = NULL;
    videoList.clear();
    if(excluding)
        videoList << excluding;
    vargusLog.writeToFile(QString("Stop play %1 return").arg(description()));
}

QList<Camera*> Set::getActiveCameras()
{
    QList<Camera *> res;
    foreach(VideoWidget *v, videoList)
    {
        if(v->isPlaying())
            res << v->getCamera();
    }
    return res;
}

void Set::next()
{
    QList<Camera *> *currentList = stc.at(tp);
    currentList->clear();
    int newFirstCam = offset[tp] * amountOfCells(tp) + lastCamNum[tp] + 1;
    lastCamNum[tp] = cameraList.length() - (newFirstCam + 1);
    if(lastCamNum[tp] >= amountOfCells(tp))
    {
        lastCamNum[tp] = amountOfCells(tp) - 1;
    }
    else if(amountOfCells(tp) > lastCamNum[tp]+1)
    {
        newFirstCam = newFirstCam - amountOfCells(tp) + lastCamNum[tp] + 1;
        lastCamNum[tp] = amountOfCells(tp) - 1;
    }
    for(int i = newFirstCam; i < cameraList.length(); i++)
    {
        currentList->push_back(cameraList.at(i));
    }
    for(int i = 0; i < newFirstCam; i++)
    {
        currentList->push_back(cameraList.at(i));
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
    int newFirstCam = offset[tp] * amountOfCells(tp);
    for(int i = newFirstCam; i < cameraList.length(); i++)
    {
        currentList->push_back(cameraList.at(i));
    }
    for(int i = 0; i < newFirstCam; i++)
    {
        currentList->push_back(cameraList.at(i));
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
    currentList->clear();
    QList<Camera *>::iterator itc = cameraList.begin();
    QList<Camera *>::iterator endc = cameraList.end();
    while(itc != endc)
    {
       currentList->push_back(*itc);
       ++itc;
    }
    offset[tp] = 0;
    int lastCam = amountOfCells(tp) - 1;
    if(lastCam >= cameraList.length())
        lastCam = cameraList.length() - 1;
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
    res << v->getCamera();
    v->stopPlay();
    stopPlay(v);
    delete layout();
    QGridLayout *grid  = new QGridLayout(this);
    setLayout(grid);
    grid->setMargin(0);
    grid->addWidget(v, 0, 0);
    v->startPlay(VideoWidget::BIGVIDEO);
    v->show();
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
    v->setCamera(c);
    connect(v, SIGNAL(bigSizeCall(VideoWidget*)), this, SLOT(bigVideo(VideoWidget*)));
    connect(v, SIGNAL(disconnectedSignal(VideoWidget*)), this, SLOT(restoreVideoWidget(VideoWidget*)));
    grid->addWidget(v, 0, 0);
    v->startPlay(VideoWidget::BIGVIDEO);
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
        (*it)->setCamera(currentList->at(i));
        (*it)->startPlay(VideoWidget::SMALLVIDEO);
    }
    it = videoList.begin();
    QList<VideoWidget *>::iterator end = videoList.end();
    while(it != end)
    {
        connect(*it,SIGNAL(bigSizeCall(VideoWidget*)),this,SLOT(bigVideo(VideoWidget*)));
        connect(*it, SIGNAL(camerasChanged(VideoWidget *, Camera *, bool)),
                this, SLOT(changeCameras(VideoWidget*, Camera*, bool)));
        connect(*it, SIGNAL(disconnectedSignal(VideoWidget*)), this, SLOT(restoreVideoWidget(VideoWidget*)));
        connect(*it, SIGNAL(arhiveCall(QString)), this, SLOT(OpenArhive(QString)));
        ++it;
    }
}

void Set::OpenArhive(QString cam)
{
    QUrl url;
    if(serverAddress.left(4).compare("http"))
    {
        url.setUrl("http://" + serverAddress + "/vargus/archive.php?camera=" + cam);
    }
    else
    {
        url.setUrl("" + serverAddress + "/vargus/archive.php?camera=" + cam);
    }
    if(url.isValid())
    {
        QDesktopServices::openUrl(url);
    }
    else
    {
        vargusLog.writeToFile("invalid arhive url" + url.toString());
    }
}

//change cameras from drag and drop
void Set::changeCameras(VideoWidget *first, Camera *second, bool fromAnotherWidget)
{
    QList<Camera *> *currentList = stc.at(tp);
    if(!fromAnotherWidget)//camera from cameralist
    {
        int f = videoList.indexOf(first);
        currentList->removeAt(f);
        currentList->insert(f, second);
        vargusLog.writeToFile("Change camera in widget "
                              + QString::number(f) + " to " + second->description());
        emit updateActiveCameras(getActiveCameras());
        wasChanged[tp] = true;
        return;
    }
    int f = videoList.indexOf(first);
    int s = 0;
    int i = 0;
    foreach(Camera *c, *currentList)
    {
        if(c == second)
        {
            s = i;
            break;
        }
        i++;
    }

    vargusLog.writeToFile("Swap widgets " + QString::number(f) + " " + QString::number(s));
    if(f >= currentList->length())
    {
        int k = f - currentList->length() + 1;
        for(int i = 0; i < k; i++)
            currentList->push_back(NULL);
        lastCamNum[tp] += k;
    }
    else if(s == currentList->length())
    {
        currentList->push_back(NULL);
        lastCamNum[tp]++;
        s = currentList->length() - 1;
    }
    currentList->swap(f, s);
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
    k = k - doubleCells.length() * 3;
    k = k - tripleCells.length() * 8;
    k = k - quadrupleCells.length() * 15;
    return k;
}

int Set::amountOfPlayingWidgets()
{
    int k = 0;
    foreach(VideoWidget *vw, videoList)
    {
        if(vw->isPlaying())
            k++;
    }
    return k;
}

void Set::enableButtons()
{
    int newFirstNextCam = offset[tp] * amountOfCells(tp) + lastCamNum[tp] + 1;
    bool nextButton = true;
    if(newFirstNextCam >= cameraList.length())
        nextButton = false;
    bool prevButton = true;
    if(offset[tp] == 0)
        prevButton = false;
    emit buttonsEnabled(prevButton, nextButton);
}

void Set::restoreVideoWidget(VideoWidget *v)
{
    vargusLog.writeToFile("Try to restore videowidget");
    v->stopPlay();
    Camera *c = v->getCamera();
    mainwindow->updateCamera(c);
    if(bigPlaying == v)
        v->startPlay(VideoWidget::BIGVIDEO);
    else
        v->startPlay(VideoWidget::SMALLVIDEO);
}


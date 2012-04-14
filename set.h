#ifndef SET_H
#define SET_H

#include <QWidget>
#include "camera.h"
#include "view.h"
#include"videowidget.h"
#include<QVector>
class Set : public QWidget
{
    Q_OBJECT

    QString set_description;
    QList<Camera *> cameraList;
    QList<View *> viewList;
    QList<VideoWidget *> videoList;

    int tp;//тип раскладки, используемой в данный момент
    bool active;
    int activeCameras;
    int* lastCamNum;
    bool buttonClicked;
    bool wasChanged;

    QVector< QList<Camera *>* > stc;//расположение камер(виджетов) под каждый view

    VideoWidget *bigPlaying;

public:
    explicit Set();
    Set(const QString &desc);
    ~Set();

    QString description(){return set_description;}
    void addView(View* view);
    void addCamera(Camera* cam);
    QStringList camerasNames();
    QList<Camera *> cameras(){return cameraList;}
    QList<View *> views(){return viewList;}
    QList<VideoWidget *> video() {return videoList; }

    void setActiveView(int index);
    void init();
    void stopPlay(VideoWidget *excluding = NULL);
    bool isActive() {return active;}
    void setActive(bool act) {active = act;}
    QList<Camera *> getActiveCameras();

    void next();
    void prev();
    void reset();
public slots:
    void restoreState() {setLayouts(tp);}
    void showBig(int num);
signals:
    void updateActiveCameras(QList<Camera*>);
private:
    void setLayouts(int type);
    void makeTwoSquare();//квадрат 2*2
    void makeFourSquareTripple();//квадрат 4*4 тройной
    void makeFourSquareOneCentral();//квадрат 4*4 двойной центральный
    void makeFiveSquareTwoOneTripple();//квадрат 5*5 два двойных один центральный
    void makeTrippleSquare();//квадрат 3*3
    void makeFourSquare();//квадрат 4*4
    int amountOfPlayingWidgets();

    void countActiveAndPlay();
    void bigVideo(Camera *c);
    int amountOfCells(int tp);

private slots:
    void updateActiveView();
    void bigVideo(VideoWidget *v);
    void changeCameras(VideoWidget *first,Camera *second, bool fromAnotherWidget);

};

#endif // SET_H

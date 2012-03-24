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

    QVector< QList<VideoWidget *>* > st;
    QVector< QList<Camera *>* > stc;


public:
    explicit Set() {activeCameras = tp = 0;}
    Set(QString desc){activeCameras = tp = 0; set_description = desc; active = false;}

    QString description(){return set_description;}
    void addView(View* view);
    void addCamera(Camera* cam);
    QStringList camerasNames();
    QList<Camera *> cameras(){return cameraList;}
    QList<View *> views(){return viewList;}

    void setActiveView(int index);
    void makeVideoWidgets();
    void restoreState() {setLayouts(tp);}
    void stopPlay();
    bool isActive() {return active;}
    void setActive(bool act) {active = act;}
    QList<Camera *> getActiveCameras();

    void next();
    void prev();
    void reset();


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
private slots:
    void updateActiveView();

};

#endif // SET_H

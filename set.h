#ifndef SET_H
#define SET_H

#include <QWidget>
#include "camera.h"
#include "view.h"
#include"videowidget.h"
#include<QVector>
#include<enums.h>

class Set : public QWidget
{
    Q_OBJECT

    QString set_description;
    QList<Camera *> cameraList;
    QList<View *> viewList;
    QList<VideoWidget *> videoList;

    int tp;//тип раскладки, используемой в данный момент
    bool active;
    int* lastCamNum;
    int* offset;
    bool buttonClicked;
    bool *wasChanged;

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
    void init(VPlayingType t);
    void stopPlay(VideoWidget *excluding = NULL);
    bool isActive() {return active;}
    void setActive(bool act) {active = act; enableButtons();}
    QList<Camera *> getActiveCameras();

    void next();
    void prev();
    void reset();

    bool setPlayingType(VPlayingType t);
public slots:
    void restoreState() { setLayouts(tp); enableButtons(); }
    void showBig(int num);
signals:
    void updateActiveCameras(QList<Camera*>);
    void buttonsEnabled(bool, bool);
private:
    void setLayouts(int type);
    int amountOfPlayingWidgets();

    void countActiveAndPlay();
    void bigVideo(Camera *c);
    int amountOfCells(int tp);

private slots:
    void updateActiveView();
    void bigVideo(VideoWidget *v);
    void changeCameras(VideoWidget *first,Camera *second, bool fromAnotherWidget);
    void enableButtons();

};

#endif // SET_H

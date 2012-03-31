#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QWidget>
#include <QUrl>
#include <QDrag>
#include <QMenu>

#include <vlc/vlc.h>
#include "camera.h"

class QFrame;

struct RunningTextSettings
{
    RunningTextSettings();
    int color;
    int opacity;
    int position;
    int refresh;
    int size;
    int timeout;
    int x;
    int y;
    int limitLine;
};

class LimitLine
{
private:
    QStringList* strings;
    int numLimitLine;
public:
    LimitLine(int _numLimitLine);
    ~LimitLine();
    void setNumLimitLine(int num);
    QString getLimitLine();
    void AddString(QString string);
    QString AddStringGetLine(QString string);
};

class VideoWidget: public QWidget
{
    Q_OBJECT
    QFrame *frame;

    QTimer *poller;
    QTimer *waitingDoubleClickTimer;
    bool isPlaying;

    static libvlc_instance_t *vlcInstance;
    libvlc_media_player_t *vlcPlayer;
    libvlc_media_t *vlcMedia;

    Camera *camera;

    QMenu* contextMenu;
    QAction *arhiveCallAction;
    void setupContextMenu();

    static RunningTextSettings* runningTextSetting;
    LimitLine* runningText;

    enum statusesClick{CLICK, DOUBLE_CLICK};
    statusesClick StatusClick;
public:
    VideoWidget();
    ~VideoWidget();
    static void staticDestructor();

    void setCamera(Camera *_camera);

    enum sizeVideo {BIGVIDEO, SMALLVIDEO};
    void startPlay(sizeVideo size);
    void stopPlay();
    void writeTextString(QString string);
    void disableTextString();
    Camera * getCamera() { return camera; }

protected:
    void mousePressEvent ( QMouseEvent * e );
    void mouseDoubleClickEvent ( QMouseEvent * event );

    void dropEvent(QDropEvent *de);
    void dragMoveEvent(QDragMoveEvent *de);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragLeaveEvent ( QDragLeaveEvent * event );
    void startDrag();
public slots:
    void updateInterface();
    void ContextMenuAction(const QPoint& z);
    void arhiveMenuPress();
    void waitingDoubleClickTimeout();
signals:
    void arhiveCall();
    void bigSizeCall(VideoWidget *);
    void camerasChanged(VideoWidget *,Camera *second, bool fromAnotherWidget);
};

#endif // VIDEOVIDGET_H

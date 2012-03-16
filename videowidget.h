#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QWidget>
#include <QUrl>
#include <QDrag>
#include <QMenu>

#include <vlc/vlc.h>

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
    bool isPlaying;

    static libvlc_instance_t *vlcInstance;
    libvlc_media_player_t *vlcPlayer;
    libvlc_media_t *vlcMedia;

    QUrl urlBigVideoStream;
    QUrl urlSmallVideoStream;

    QMenu* contextMenu;
    QAction *arhiveCallAction;
    void setupContextMenu();

    static RunningTextSettings* runningTextSetting;
    LimitLine* runningText;
public:
    VideoWidget();
    ~VideoWidget();

    enum sizeVideo {BIGVIDEO, SMALLVIDEO};
    void setUrlVideoStream(QUrl urlStream, sizeVideo size);

    void startPlay(sizeVideo size);
    void stopPlay();
    void writeTextString(QString string);
    void disableTextString();

protected:
    void mousePressEvent ( QMouseEvent * e );

    void dropEvent(QDropEvent *de);
    void dragMoveEvent(QDragMoveEvent *de);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragLeaveEvent ( QDragLeaveEvent * event );
public slots:
    void updateInterface();
    void ContextMenuAction(const QPoint& z);
    void arhiveMenuPress();
signals:
    void arhiveCall();
};

#endif // VIDEOVIDGET_H

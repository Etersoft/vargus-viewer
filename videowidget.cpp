#include "videowidget.h"

#include <QVBoxLayout>
#include <QFrame>
#include <QTimer>

#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>

#include <QApplication>

libvlc_instance_t *VideoWidget::vlcInstance = 0;
RunningTextSettings *VideoWidget::runningTextSetting = 0;
RunningTextSettings::RunningTextSettings()
{
    color = 0xFFFFFF;
    opacity = 255;
    position = -1;
    refresh = 1000;
    size = -1;
    timeout = 0;
    x = 0;
    y = 0;
    limitLine = 13;
}

LimitLine::LimitLine(int _numLimitLine) : numLimitLine(_numLimitLine)
{
    strings = new QStringList();
}

LimitLine::~LimitLine()
{
    delete strings;
}

void LimitLine::AddString(QString string)
{
    strings->append(string);
    if(strings->count() > numLimitLine)
    {
        //strings->erase();
        //strings->first().clear();
        strings->pop_front();
    }
}

QString LimitLine::getLimitLine()
{
    QString finalResult = QString("");
    foreach(const QString &sringElement, *strings)
    {
        finalResult += sringElement + "\n";
    }
    return finalResult;
}

QString LimitLine::AddStringGetLine(QString string)
{
    AddString(string);
    return getLimitLine();
}

VideoWidget::VideoWidget(): QWidget()
{

    const char * const vlc_args[] = {
              "-I", "dummy", /* Don't use any interface */
              "--ignore-config", /* Don't use VLC's config */
              "--no-audio", /* Audio off */
#ifdef QT_DEBUG
              "--extraintf=logger", /* log anything */
              "--verbose=2", /* be much more verbose then normal for debugging purpose */
#endif
             "" }; /* "--no-video-title-show" */


    isPlaying=false;
    frame=new QFrame(this);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(frame);
    setLayout(layout);

    poller=new QTimer(this);

    if(!vlcInstance)
    {
        vlcInstance=libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);
    }
    if(!runningTextSetting)
    {
        runningTextSetting = new RunningTextSettings();
    }

    runningText = new LimitLine(runningTextSetting->limitLine) ;

    vlcPlayer = libvlc_media_player_new (vlcInstance);

    connect(poller, SIGNAL(timeout()), this, SLOT(updateInterface()));
    setAcceptDrops(true);
    poller->start(100);

    waitingDoubleClickTimer = new QTimer();

    setupContextMenu();
}

VideoWidget::~VideoWidget()
{
    libvlc_media_player_release (vlcPlayer);
    libvlc_media_release(vlcMedia);
}

void  VideoWidget::staticDestructor()
{
    libvlc_release(vlcInstance);
}

void VideoWidget::setCamera(Camera* _camera)
{
    camera = _camera;
}

void VideoWidget::startPlay(sizeVideo size)
{
    switch(size)
    {
        case BIGVIDEO:
            vlcMedia = libvlc_media_new_path(vlcInstance, camera->source().toAscii());
            break;
        case SMALLVIDEO:
            vlcMedia = libvlc_media_new_path(vlcInstance, camera->preview().toAscii());
            break;
    }

    libvlc_media_player_set_media (vlcPlayer, vlcMedia);

    //#FIXME For linux only
    int windid = frame->winId();
    libvlc_media_player_set_xwindow (vlcPlayer, windid );

    int ret = libvlc_media_player_play (vlcPlayer);
    if(ret == 0)
        isPlaying=true;
}

void VideoWidget::stopPlay()
{
    if(isPlaying)
        libvlc_media_player_stop(vlcPlayer);
}

void VideoWidget::updateInterface()
{
    if(!isPlaying)
        return;

    libvlc_media_t *curMedia = libvlc_media_player_get_media (vlcPlayer);
    if (curMedia == NULL)
        return;
}

void VideoWidget::mousePressEvent ( QMouseEvent * e )
{
    if(e->button() == Qt::RightButton)
    {
        ContextMenuAction(e->pos());
    }

    if(e->button() != Qt::LeftButton)
        return;

    StatusClick =  CLICK;

    connect(waitingDoubleClickTimer, SIGNAL(timeout()), this, SLOT(waitingDoubleClickTimeout()));
    waitingDoubleClickTimer->start(QApplication::doubleClickInterval());
}

void VideoWidget::startDrag()
{
    QDrag* drag = new QDrag(this);
    // The data to be transferred by the drag and drop operation is contained in a QMimeData object

    QMimeData *data = new QMimeData;
    data->setUserData(0,(QObjectUserData*)camera);
    data->setUserData(1,(QObjectUserData*)this);

    // Assign ownership of the QMimeData object to the QDrag object.
    drag->setMimeData(data);
    // Start the drag and drop operation
    drag->start();
}

void VideoWidget::mouseDoubleClickEvent ( QMouseEvent * event )
{
    StatusClick =  DOUBLE_CLICK;
    emit bigSizeCall();
}

void VideoWidget::waitingDoubleClickTimeout()
{
    waitingDoubleClickTimer->stop();
    if(StatusClick ==  CLICK)
    {
        startDrag();
    }
}

void VideoWidget::dropEvent(QDropEvent *de)
{
   VideoWidget* dragVideoWindet;
   dragVideoWindet = (VideoWidget*)de->mimeData()->userData(1);
   if(dragVideoWindet)
   {
        dragVideoWindet->stopPlay();
        dragVideoWindet->setCamera(camera);
        dragVideoWindet->startPlay(SMALLVIDEO);
   }

   Camera *dragCamera;
   dragCamera = (Camera*)de->mimeData()->userData(0);
   this->stopPlay();
   this->setCamera(dragCamera);
   this->startPlay(SMALLVIDEO);

   startPlay(SMALLVIDEO);
}

void VideoWidget::dragMoveEvent(QDragMoveEvent *de)
{
    de->accept();
}

void VideoWidget::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void VideoWidget::dragLeaveEvent ( QDragLeaveEvent * event )
{
    //stopPlay();
    int i =0;
}

void VideoWidget::setupContextMenu()
{
    frame->setContextMenuPolicy(Qt::CustomContextMenu);
    contextMenu=new QMenu(this);
    arhiveCallAction=new QAction(this);
    arhiveCallAction->setText(trUtf8("Архив"));

    connect(frame, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(ContextMenuAction(const QPoint& z)));
}

void VideoWidget::ContextMenuAction(const QPoint& z)
{
    contextMenu->clear();
    disconnect(arhiveCallAction, 0, 0, 0);
    contextMenu->addAction(arhiveCallAction);
    connect(arhiveCallAction, SIGNAL(triggered()), this,SLOT(arhiveMenuPress()));
    contextMenu->exec(mapToGlobal(z));
}

void VideoWidget::arhiveMenuPress()
{
    emit arhiveCall();
}

void VideoWidget::writeTextString(QString string)
{
    libvlc_video_set_marquee_int(vlcPlayer,libvlc_marquee_Color,runningTextSetting->color);
    libvlc_video_set_marquee_int(vlcPlayer,libvlc_marquee_Opacity,runningTextSetting->opacity);
    libvlc_video_set_marquee_int(vlcPlayer,libvlc_marquee_Position,runningTextSetting->position);
    libvlc_video_set_marquee_int(vlcPlayer,libvlc_marquee_Refresh,runningTextSetting->refresh);
    libvlc_video_set_marquee_int(vlcPlayer,libvlc_marquee_Size,runningTextSetting->size);
    libvlc_video_set_marquee_int(vlcPlayer,libvlc_marquee_Timeout,runningTextSetting->timeout);
    libvlc_video_set_marquee_int(vlcPlayer,libvlc_marquee_X,runningTextSetting->x);
    libvlc_video_set_marquee_int(vlcPlayer,libvlc_marquee_Y,runningTextSetting->y);

    libvlc_video_set_marquee_int(vlcPlayer,libvlc_marquee_Enable,1);
    libvlc_video_set_marquee_string(vlcPlayer,libvlc_marquee_Text,runningText->AddStringGetLine(string).toAscii());
}

void VideoWidget::disableTextString()
{
    libvlc_video_set_marquee_int(vlcPlayer,libvlc_marquee_Enable,0);
}

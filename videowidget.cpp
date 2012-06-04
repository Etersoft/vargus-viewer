#include "videowidget.h"

#include <QVBoxLayout>
#include <QFrame>
#include <QTimer>

#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>

#include <QApplication>
#include <QtCore>
#include"logger.h"
extern Logger &vargusLog;

libvlc_instance_t *VideoWidget::vlcInstance = 0;
RunningTextSettings *VideoWidget::runningTextSetting = 0;
RunningString *VideoWidget::runningString = 0;
QString VideoWidget::runningTextip = "";
int VideoWidget::runningTextport = 0;
VPlayingType VideoWidget::pltp = LOWLEVEL;

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
        strings->pop_front();
    }
}

QString LimitLine::getLimitLine()
{
    QString finalResult = QString("");
    foreach(const QString &sringElement, *strings)
    {
        finalResult += sringElement + "";
    }
    return finalResult;
}

QString LimitLine::AddStringGetLine(QString string)
{
    AddString(string);
    return getLimitLine();
}

VideoWidget::VideoWidget(): VideoWidgetLowLevelPainting()
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

    isPlaying = false;
    frame = new QFrame(this);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(frame);
    setLayout(layout);

    poller=new QTimer(this);

    if(!vlcInstance)
    {
        vlcInstance=libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);
    }
    if(!runningString)
    {
        runningString = new RunningString(runningTextip, runningTextport);
    }
    if(!runningTextSetting)
    {
        runningTextSetting = new RunningTextSettings();
    }
    vlcMedia = NULL;
    camera = NULL;

    runningText = new LimitLine(runningTextSetting->limitLine) ;

    vlcPlayer = libvlc_media_player_new (vlcInstance);

    connect(poller, SIGNAL(timeout()), this, SLOT(updateInterface()));
    setAcceptDrops(true);
    poller->start(5000);

    waitingDoubleClickTimer = new QTimer();

    setupContextMenu();
}

VideoWidget::~VideoWidget()
{
    vargusLog.writeToFile("destroy VideoWidget()");
    this->hide();
    stopPlay();
    clearVlc(vlcPlayer, vlcMedia);
}

void VideoWidget::clearVlc(libvlc_media_player_t *vlcPlayer,libvlc_media_t *vlcMedia)
{
    libvlc_media_player_stop(vlcPlayer);
    libvlc_media_player_release (vlcPlayer);
    if(vlcMedia)
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
    vargusLog.writeToFile("startPlay");

    if(camera == NULL)
           return;
    switch(size)
    {
        case BIGVIDEO:
            vlcMedia = libvlc_media_new_path(vlcInstance, camera->source().toAscii());
            break;
        case SMALLVIDEO:
            vlcMedia = libvlc_media_new_path(vlcInstance, camera->preview().toAscii());
            break;
    }
    libvlc_media_set_meta (vlcMedia,  libvlc_meta_Title, camera->name().toAscii());
    libvlc_media_player_set_media (vlcPlayer, vlcMedia);

    //Set this class for write camera events
    runningString->addPrintMethod(camera->name(),this);

    //#FIXME For linux only
    int ret = 0;
    if(pltp == XWINDOW)
    {
        vargusLog.writeToFile("stas XWINDOW");
        int windid = frame->winId();
        vargusLog.writeToFile("stas windid " + QString::number(windid));
        if(windid)
        {
            libvlc_media_player_set_xwindow (vlcPlayer, windid );
            ret = libvlc_media_player_play (vlcPlayer);
        }
    }

    if(pltp == LOWLEVEL)
    {
        vargusLog.writeToFile("stas LOWLEVEL");
        activateLowLevelPainting();
        ret = libvlc_media_player_play (vlcPlayer);
    }
    isPlaying=true;
}

void VideoWidget::stopPlay()
{
    if(isPlaying)
    {
        libvlc_media_player_stop(vlcPlayer);
        if(pltp == LOWLEVEL)
            deactivateLowLevelPainting();
    }
    isPlaying = false;
}

void VideoWidget::startvlcPlayer()
{
    libvlc_media_player_play(vlcPlayer);
}

void VideoWidget::stoptvlcPlayer()
{
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
    if(camera == NULL)
        return;
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
    emit bigSizeCall(this);
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
   bool fromAnotherWidget = false;
   dragVideoWindet = (VideoWidget*)de->mimeData()->userData(1);
   Camera *dragCamera;
   dragCamera = (Camera*)de->mimeData()->userData(0);
   if(dragVideoWindet)
   {
       if(this == dragVideoWindet)
           return;
        dragVideoWindet->stopPlay();
        dragVideoWindet->setCamera(camera);
        dragVideoWindet->startPlay(SMALLVIDEO);
        fromAnotherWidget = true;
   }
   stopPlay();
   camera = dragCamera;
   startPlay(SMALLVIDEO);
   emit camerasChanged(this, dragCamera, fromAnotherWidget);
}

void VideoWidget::drawImage()
{

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
    //empty event
}

void VideoWidget::setupContextMenu()
{
    frame->setContextMenuPolicy(Qt::CustomContextMenu);
    contextMenu=new QMenu(this);
    arhiveCallAction=new QAction(this);
    arhiveCallAction->setText(tr("Archive"));

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

void VideoWidget::printString(QString rString)
{
    writeTextString( rString );
}

void VideoWidget::writeTextString(QString string)
{
    vargusLog.writeToFile("write text string " + string);
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

void VideoWidget::changeTextServerSettings(const QString &_adress, int _port)
{
    if(runningString)
        runningString -> changeConnection(_adress, _port);
    else
        runningString = new RunningString(_adress, _port);
}

libvlc_media_player_t * VideoWidget::getvlcPlayer()
{
    return vlcPlayer;
}

void VideoWidget::setRunningTextAddress(QString ip, int port)
{
    runningTextip = ip;
    runningTextport = port;
}

void VideoWidget::setVPlayingType(VPlayingType pt)
{
    pltp = pt;
}

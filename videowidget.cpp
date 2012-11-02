#include "videowidget.h"

#include <QVBoxLayout>
#include <QFrame>
#include <QTimer>

#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>

#include <QApplication>
#include <QtCore>
#include <QLineEdit>
#include"logger.h"
extern Logger &vargusLog;

libvlc_instance_t *VideoWidget::vlcInstance = 0;
const char **VideoWidget::VlcArgs = 0;
int VideoWidget::numVlcArgs = 0;


void VideoWidget::setVlcArgs( const char *const *_vlcArgs, int _numberVlcArgs)
{
    VlcArgs = (const char **)_vlcArgs;
    numVlcArgs = _numberVlcArgs;
}

VideoWidget::VideoWidget(): VideoWidgetLowLevelPainting()
{
    isPlaying = false;
    isRunningStringActive = true;
    isStillPlay = false;
    StatusClick = NO_CLICK;
    isStatusNoSignal = false;
    statusLabel = 0;
    frame = new QFrame(this);
    frame->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    //layout = new QVBoxLayout(this);
    layout = new QVBoxLayout();
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(frame);
    setLayout(layout);

    poller=new QTimer(this);

    if(!vlcInstance)
    {
        for(int i = 0; i < numVlcArgs; i++ )
        {
            if(strncmp(VlcArgs[i], "--video-title-timeout",strlen("--video-title-timeout")) == 0)
            {
                VlcArgs[i] = "--video-title-timeout=1";
            }
        }
        vlcInstance = libvlc_new(numVlcArgs, VlcArgs);
    }

    vlcMedia = NULL;
    camera = NULL;

    vlcPlayer = libvlc_media_player_new (vlcInstance);


    setAcceptDrops(true);
    poller->start(5000);
    waitingDoubleClickTimer = new QTimer();

    setupContextMenu();
}

VideoWidget::~VideoWidget()
{
    if(camera)
        vargusLog.writeToFile(
                    QString("destroy VideoWidget() %1").arg(camera->description()));
    else
        vargusLog.writeToFile("destroy VideoWidget()");
    /*if(isVisible())
        this->hide(); это нельзя использовать,
    так как поток удаления может ещё удалять старые виджеты,
    хотя родитель и менеджер компоновки уже удалены*/
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
            setAspectComply(true);
            break;
        case SMALLVIDEO:
            vlcMedia = libvlc_media_new_path(vlcInstance, camera->preview().toAscii());
            setAspectComply(false);
            break;
    }
    vargusLog.writeToFile("disconnectAction");

    printedTitle = camera->name() + ":" + camera->description();

    libvlc_media_player_set_media (vlcPlayer, vlcMedia);

    //Set this class for write camera events


    //#FIXME For linux only
    vargusLog.writeToFile("stas LOWLEVEL");
    activateLowLevelPainting();
    int ret = libvlc_media_player_play (vlcPlayer);
    vlcSetEvent();
    camera->runningString->addPrintMethod(camera->name(),this);
    vargusLog.writeToFile("Start play ret " + QString::number(ret));
    isPlaying=true;
    isRunningStringActive = true;
    int isp = libvlc_media_player_is_playing (vlcPlayer);
    vargusLog.writeToFile("startPlay isp " + QString::number(isp));
    afterStart = 0;
    connect(poller, SIGNAL(timeout()), this, SLOT(updateInterface()));
}

void VideoWidget::stopPlay()
{
    if(isPlaying)
    {
        paintblank();
        disconnect(poller, SIGNAL(timeout()), 0, 0);
        vlcSetOffEvent();
        isPlaying = false;
        isRunningStringActive = false;
        camera->runningString->dropPrintMethod(camera->name());
        libvlc_media_player_stop(vlcPlayer);
        deactivateLowLevelPainting();
    }
}

void VideoWidget::startvlcPlayer()
{
    isPlaying = true;
    isRunningStringActive = true;
    libvlc_media_player_play(vlcPlayer);
}

void VideoWidget::stoptvlcPlayer()
{
    isPlaying = false;
    isRunningStringActive = false;
    libvlc_media_player_stop(vlcPlayer);
}

void VideoWidget::updateInterface()
{
    if(!isPlaying)
        return;

    if(afterStart<2)
    {
        printString();
        afterStart++;
    }

    libvlc_media_t *curMedia = libvlc_media_player_get_media (vlcPlayer);
    if (curMedia == NULL)
        return;

    int isp = libvlc_media_player_is_playing (vlcPlayer);
    if(!isp)
    {
        emit disconnectedSignal(this);
        setNosignalMessage();
        return;
    }

    if(!isStillPlay)
    {
        setNosignalMessage();
    }
    else
    {
        setOffNosignalMessage();
    }

    isStillPlay = false;
}

void VideoWidget::setNosignalMessage()
{
    if(!isStatusNoSignal)
    {
        statusLabel = new QLabel();
        paintNoSignal();
    }
    isStatusNoSignal = true;
}

void VideoWidget::setOffNosignalMessage()
{
    if(isStatusNoSignal)
    {
        paintNothing();
        delete statusLabel;
    }
    isStatusNoSignal = false;
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

void VideoWidget::mouseReleaseEvent (QMouseEvent *)
{
    StatusClick =  NO_CLICK;
}

void VideoWidget::startDrag()
{
    if(camera == NULL)
        return;
    QDrag* drag = new QDrag(this);
    // The data to be transferred by the drag and drop operation is contained in a QMimeData object
    vargusLog.writeToFile("Drag proc " + QString::number(QCoreApplication::applicationPid()));

    QMimeData *data = new QMimeData;
    data->setText(QString("varguscamera") + QString::number(QCoreApplication::applicationPid()));
    data->setUserData(0, (QObjectUserData*)camera);
    data->setUserData(1, (QObjectUserData*)this);

    // Assign ownership of the QMimeData object to the QDrag object.
    drag->setMimeData(data);
    // Start the drag and drop operation
    drag->start();
}

void VideoWidget::mouseDoubleClickEvent ( QMouseEvent * )
{
    StatusClick = DOUBLE_CLICK;
    if(isPlaying)
        emit bigSizeCall(this);
}

void VideoWidget::waitingDoubleClickTimeout()
{
    waitingDoubleClickTimer->stop();
    if(StatusClick ==  CLICK)
    {
        StatusClick = NO_CLICK;
        startDrag();
    }
}

void VideoWidget::dropEvent(QDropEvent *de)
{
   VideoWidget* dragVideoWindet;
   bool fromAnotherWidget = false;

   if(!de->mimeData()->hasText())
       return;
   QString checktext = de->mimeData()->text();
   if(checktext.compare(QString("varguscamera") + QString::number(QCoreApplication::applicationPid())))
       return;
   vargusLog.writeToFile("Drop proc " +  QString::number(QCoreApplication::applicationPid()));

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

void VideoWidget::dragLeaveEvent ( QDragLeaveEvent * )
{
    //empty event
}

void VideoWidget::setupContextMenu()
{
    frame->setContextMenuPolicy(Qt::CustomContextMenu);
    contextMenu=new QMenu(this);
    arhiveCallAction=new QAction(this);
    arhiveCallAction->setText(tr("Archive"));
    changeStateMessageWidgetAction=new QAction(this);

    changeStateMessageCameraAction=new QAction(this);

    connect(frame, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(ContextMenuAction(const QPoint& z)));
}

void VideoWidget::ContextMenuAction(const QPoint& z)
{
    if(isRunningStringActive)
        changeStateMessageWidgetAction->setText(tr("Disable message on widget"));
    else
        changeStateMessageWidgetAction->setText(tr("Enable message on widget"));

    if(camera->isRunningStringActive)
        changeStateMessageCameraAction->setText(tr("Disable message on camera"));
    else
        changeStateMessageCameraAction->setText(tr("Enable message on camera"));

    contextMenu->clear();
    disconnect(arhiveCallAction, 0, 0, 0);
    disconnect(changeStateMessageWidgetAction, 0, 0, 0);
    disconnect(changeStateMessageCameraAction, 0, 0, 0);

    contextMenu->addAction(arhiveCallAction);
    contextMenu->addAction(changeStateMessageWidgetAction);
    contextMenu->addAction(changeStateMessageCameraAction);

    connect(arhiveCallAction, SIGNAL(triggered()), this,SLOT(arhiveMenuPress()));
    connect(changeStateMessageWidgetAction, SIGNAL(triggered()), this, SLOT(changeStateMessageWidgetPress()));
    connect(changeStateMessageCameraAction, SIGNAL(triggered()), this, SLOT(changeStateMessageCameraPress()));
    contextMenu->exec(mapToGlobal(z));
}

void VideoWidget::arhiveMenuPress()
{
    emit arhiveCall(camera->name());
}

void VideoWidget::changeStateMessageWidgetPress()
{
    isRunningStringActive = !isRunningStringActive;
}

void VideoWidget::changeStateMessageCameraPress()
{
    camera->isRunningStringActive = !camera->isRunningStringActive;
}

void VideoWidget::printString()
{
}

bool VideoWidget::isneedPrintTextEvents()
{
    return (isRunningStringActive && camera->isRunningStringActive && libvlc_media_player_is_playing (vlcPlayer));
}

QString VideoWidget::getTextEvent()
{
    return camera->runningText->getLimitLine();
}

void VideoWidget::disableTextString()
{
}

libvlc_media_player_t * VideoWidget::getvlcPlayer()
{
    return vlcPlayer;
}

void eventCallback(const libvlc_event_t *, void *p_user_data)
{
    VideoWidget* vw = (VideoWidget*)p_user_data;
    vw->setStillPlay();
}

void VideoWidget::setStillPlay()
{
    isStillPlay = true;
}

void VideoWidget::vlcSetEvent()
{
    libvlc_event_manager_t* vlcEventManager = libvlc_media_player_event_manager(vlcPlayer);
    libvlc_event_attach(vlcEventManager, libvlc_MediaPlayerPositionChanged, eventCallback, this);
}

void VideoWidget::vlcSetOffEvent()
{
    libvlc_event_manager_t* vlcEventManager = libvlc_media_player_event_manager(vlcPlayer);
    libvlc_event_detach(vlcEventManager, libvlc_MediaPlayerPositionChanged, eventCallback, this);
}












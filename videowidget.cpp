#include "videowidget.h"

#include <QVBoxLayout>
#include <QFrame>
#include <QTimer>

#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>

VideoWidget::VideoWidget(): QWidget()
{
    const char * const vlc_args[] = {
              "-I", "dummy", /* Don't use any interface */
              "--ignore-config", /* Don't use VLC's config */
              "--extraintf=logger", //log anything
              "--verbose=2", //be much more verbose then normal for debugging purpose
             "" }; /* "--no-video-title-show"*/


    _isPlaying=false;
    frame=new QFrame(this);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(frame);
    setLayout(layout);

    poller=new QTimer(this);

    vlcInstance=libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);
    vlcPlayer = libvlc_media_player_new (vlcInstance);

    connect(poller, SIGNAL(timeout()), this, SLOT(updateInterface()));
    setAcceptDrops(true);
    poller->start(100);
}

VideoWidget::~VideoWidget()
{
/*
    libvlc_media_player_stop (vlcPlayer);

    libvlc_media_release(vlcMedia);
    libvlc_media_player_release (vlcPlayer);
    libvlc_release (vlcInstance);
    */
}

void VideoWidget::setUrlVideoStream(QUrl urlStream, sizeVideo size)
{
    switch(size)
    {
        case BIGVIDEO:
            urlBigVideoStream = urlStream;
            break;
        case SMALLVIDEO:
            urlSmallVideoStream = urlStream;
            break;
    }
}


void VideoWidget::startPlay(sizeVideo size)
{
    switch(size)
    {
        case BIGVIDEO:
            vlcMedia = libvlc_media_new_path(vlcInstance, urlBigVideoStream.toString().toAscii());
            break;
        case SMALLVIDEO:
            vlcMedia = libvlc_media_new_path(vlcInstance, urlSmallVideoStream.toString().toAscii());
            break;
    }

    libvlc_media_player_set_media (vlcPlayer, vlcMedia);

    //#FIXME For linux only
    int windid = frame->winId();
    libvlc_media_player_set_xwindow (vlcPlayer, windid );

    int ret = libvlc_media_player_play (vlcPlayer);
    if(ret == 0)
        _isPlaying=true;
}

void VideoWidget::stopPlay()
{
    if(_isPlaying)
        libvlc_media_player_stop(vlcPlayer);
}

void VideoWidget::updateInterface()
{
    if(!_isPlaying)
        return;

    libvlc_media_t *curMedia = libvlc_media_player_get_media (vlcPlayer);
    if (curMedia == NULL)
        return;
}

void VideoWidget::mousePressEvent ( QMouseEvent * e )
{
    QDrag* drag = new QDrag(this);
    // The data to be transferred by the drag and drop operation is contained in a QMimeData object
    QList<QUrl> *urls= new QList<QUrl>;
    urls->append(urlBigVideoStream);
    urls->append(urlSmallVideoStream);

    QMimeData *data = new QMimeData;
    data->setUrls(*urls);
    // Assign ownership of the QMimeData object to the QDrag object.
    drag->setMimeData(data);
    // Start the drag and drop operation
    drag->start();
}

void VideoWidget::dropEvent(QDropEvent *de)
{
   stopPlay();
   QList<QUrl> urls;
   urls = de->mimeData()->urls();
   urlBigVideoStream = urls.takeAt(0);
   urlSmallVideoStream = urls.takeAt(0);
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


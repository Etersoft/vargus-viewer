#include "dragtextedit.h"
#include <QUrl>

#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>


DragTextEdit::DragTextEdit(QWidget * parent) : QLineEdit(parent)
{

}

void DragTextEdit::mousePressEvent ( QMouseEvent * e )
{
    QDrag* drag = new QDrag(this);

    QList<QUrl> *urls= new QList<QUrl>;
    urls->append(QUrl(this->text()));
    urls->append(QUrl(this->text()));

    QMimeData *data = new QMimeData;
    data->setUrls(*urls);
    drag->setMimeData(data);
    drag->start();
}

void DragTextEdit::dragMoveEvent(QDragMoveEvent *de)
{
    de->accept();
}

void DragTextEdit::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void DragTextEdit::dragLeaveEvent ( QDragLeaveEvent * event )
{

}

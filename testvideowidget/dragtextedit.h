#ifndef DRAGTEXTEDIT_H
#define DRAGTEXTEDIT_H

#include <QLineEdit>

class DragTextEdit : public QLineEdit
{
    Q_OBJECT
public:
    DragTextEdit(QWidget * parent);
protected:
    void mousePressEvent ( QMouseEvent * e );

    void dragMoveEvent(QDragMoveEvent *de);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragLeaveEvent ( QDragLeaveEvent * event );
};

#endif // DRAGTEXTEDIT_H

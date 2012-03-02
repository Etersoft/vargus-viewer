#ifndef VIEW_H
#define VIEW_H

#include <QObject>
#include <QList>
#include <QPixmap>
#include <QStringList>

class View : public QObject
{
    Q_OBJECT

    QString view_description;
    int view_width;
    int view_height;
    QList<int> view_double;
    QList<int> view_triple;
    QList<int> view_quadruple;

    QPixmap *view_icon;

public:
    View();
    View(QString new_value){view_description = new_value;}

    void setDescription(QString new_value){view_description = new_value;}
    void setWidth(int new_value){view_width = new_value;}
    void setHeight(int new_value){view_height = new_value;}
    void setDoubleFrames(QStringList list);
    void setTripleFrames(QStringList list);
    void setQuadrupleFrames(QStringList list);

    void createIcon();
};

#endif // VIEW_H

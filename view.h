#ifndef VIEW_H
#define VIEW_H

#include <QObject>
#include <QPushButton>
#include <QList>
#include <QPixmap>
#include <QStringList>
#include <QIcon>

#define ICON_WIDTH 90
#define ICON_HEIGHT 60

class View : public QPushButton
{
    Q_OBJECT

    QString view_description;
    int view_width;
    int view_height;
    QList<int> view_double;
    QList<int> view_triple;
    QList<int> view_quadruple;

    QIcon *view_icon;
    QIcon *view_active_icon;

    bool active;

public:
    View();
    View(QString new_value);

    void setDescription(QString new_value){view_description = new_value;}
    void setWidth(int new_value){view_width = new_value;}
    void setHeight(int new_value){view_height = new_value;}
    void setDoubleFrames(QStringList list);
    void setTripleFrames(QStringList list);
    void setQuadrupleFrames(QStringList list);

    QString description(){return view_description;}
    int width(){return view_width;}
    int height(){return view_height;}
    QList<int> doubleCells(){return view_double;}
    QList<int> tripleCells(){return view_triple;}
    QList<int> quadrupleCells(){return view_quadruple;}

    void createIcons();

private:
    QPixmap createIconImage(QColor color, QColor bkColor);

private slots:
};

#endif // VIEW_H

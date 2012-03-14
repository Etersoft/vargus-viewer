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
    bool waitActive;

public:
    View();
    View(QString new_value);
    View(View* view);

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
    QIcon* activeIcon(){return view_active_icon;}
    QIcon* passiveIcon(){return view_icon;}

    void setActive(bool a);
    bool waitingActive(){return waitActive;}
    bool updateActivity();

private:
    QPixmap createIconImage(QColor color, QColor bkColor);

private slots:
    void onClick();

signals:
    void waitForUpdate();
};

#endif // VIEW_H

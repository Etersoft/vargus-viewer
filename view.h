/*
Store view icons

Copyright (C) 2012-2013 Etersoft <info@etersoft.ru>

This file is part of VargusViewer.

VargusViewer is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

VargusViewer is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

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
    explicit View(QString new_value);
    explicit View(View* view);
    ~View();

    void setDescription(const QString &new_value){view_description = new_value;}
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

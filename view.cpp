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

#include "view.h"
#include <QPainter>
#include "logger.h"

extern Logger &vargusLog;

View::View(QString new_value)
{
    view_description = new_value;
    view_active_icon = view_icon = NULL;
    view_width = 0;
    view_height = 0;
    active = false;
    waitActive = false;
}
View::~View()
{
    if(view_icon != NULL)
        delete view_icon;
    if(view_active_icon != NULL)
        delete view_active_icon;
}

View::View(View* view)
{
    view_description = view->description();
    view_width = view->width();
    view_height = view->height();
    view_double = view->doubleCells();
    view_triple = view->tripleCells();
    view_quadruple = view->quadrupleCells();
    view_icon = new QIcon(*view->passiveIcon());
    view_active_icon = new QIcon(*view->activeIcon());

    active = false;
    waitActive = false;

    setIcon(*view_icon);
    setFixedSize(ICON_WIDTH, ICON_HEIGHT);
    setIconSize(size());
    setToolTip(view_description);

    connect(this, SIGNAL(clicked()), this, SLOT(onClick()));
}

void View::setDoubleFrames(QStringList list)
{
    while(!list.isEmpty())
    {
        QString value = list.takeAt(0);
        if(!value.isEmpty())
            view_double << value.toInt();
    }
}

void View::setTripleFrames(QStringList list)
{
    while(!list.isEmpty())
    {
        QString value = list.takeAt(0);
        if(!value.isEmpty())
            view_triple << value.toInt();
    }
}

void View::setQuadrupleFrames(QStringList list)
{
    while(!list.isEmpty())
    {
        QString value = list.takeAt(0);
        if(!value.isEmpty())
            view_quadruple << value.toInt();
    }
}

void View::createIcons()
{
    view_icon = new QIcon(createIconImage(Qt::blue, Qt::gray));
    view_active_icon = new QIcon(createIconImage(Qt::blue, Qt::white));

    setIcon(*view_icon);
    setFixedSize(ICON_WIDTH, ICON_HEIGHT);
    setIconSize(size());
}

QPixmap View::createIconImage(QColor color, QColor bkColor)
{
    QPixmap image(ICON_WIDTH, ICON_HEIGHT);

    QPainter painter;
    painter.begin(&image);
    painter.setPen(color);
    painter.fillRect(0, 0, ICON_WIDTH - 1, ICON_HEIGHT - 1, bkColor);

    // Габариты ячейки
    int cellwidth = (ICON_WIDTH - view_width - 1) / view_width + 1;
    int cellheight = (ICON_HEIGHT - view_height - 1) / view_height + 1;

    // Относительное начало координат (с учётом сдвинутых ячеек)
    int nullx = (ICON_WIDTH - cellwidth * view_width - view_width - 1) / 2;
    int nully = (ICON_HEIGHT - cellheight * view_height - view_height - 1) / 2;

    // Рисование сетки
    for(int i = 0; i < view_width; i++)
    {
        for(int j = 0; j < view_height; j++)
        {
            painter.drawRect(nullx + i + i * cellwidth, nully + j + j * cellheight, cellwidth + 1, cellheight + 1);
        }
    }

    // Отрисовка двойных квадратов
    if(!view_double.isEmpty())
    {
        for(int i = 0; i < view_double.count(); i++)
        {
            int doubleCell = view_double.at(i);
            int dx = doubleCell % view_width - 1;
            int dy = doubleCell / view_width;
            painter.fillRect(nullx + dx + dx * cellwidth + 1, nully + dy + dy * cellheight + 1, 2 * cellwidth + 1, 2 * cellheight + 1, bkColor);
        }
    }

    // Отрисовка тройных квадратов
    if(!view_triple.isEmpty())
    {
        for(int i = 0; i < view_triple.count(); i++)
        {
            int tripleCell = view_triple.at(i);
            int dx = tripleCell % view_width - 1;
            int dy = tripleCell / view_width;
            painter.fillRect(nullx + dx + dx * cellwidth + 1, nully + dy + dy * cellheight + 1, 3 * cellwidth + 2, 3 * cellheight + 2, bkColor);
        }
    }

    // Отрисовка четверных квадратов
    if(!view_quadruple.isEmpty())
    {
        for(int i = 0; i < view_quadruple.count(); i++)
        {
            int quadrupleCell = view_quadruple.at(i);
            int dx = quadrupleCell % view_width - 1;
            int dy = quadrupleCell / view_width;
            painter.fillRect(nullx + dx + dx * cellwidth + 1, nully + dy + dy * cellheight + 1, 4 * cellwidth + 3, 4 * cellheight + 3, bkColor);
        }
    }

    // Обводка
    painter.drawRect(0, 0, ICON_WIDTH - 1, ICON_HEIGHT - 1);

    painter.end();

    return image;
}

void View::setActive(bool a)
{
    active = a;
    if(active)
        setIcon(*view_active_icon);
    else
        setIcon(*view_icon);
}

void View::onClick()
{
    vargusLog.writeToFile("View clicked " + view_description);
    waitActive = true;
    emit(waitForUpdate());
}

bool View::updateActivity()
{
    this->setActive(waitActive);
    waitActive = false;
    return active;
}

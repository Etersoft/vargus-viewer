#include "view.h"
#include <QPainter>

View::View()
{
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

    this->setIcon(*view_icon);
    this->setFixedSize(ICON_WIDTH, ICON_HEIGHT);
    this->setIconSize(this->size());
}

QPixmap View::createIconImage(QColor color, QColor bkColor)
{
    QPixmap image(ICON_WIDTH, ICON_HEIGHT);

    QPainter painter;
    painter.begin(&image);
    painter.setPen(color);
    painter.fillRect(0, 0, ICON_WIDTH - 1, ICON_HEIGHT - 1, bkColor);

    // Габариты ячейки
    int cellwidth = ( ICON_WIDTH - view_width - 1) / view_width + 1;
    int cellheight = ( ICON_HEIGHT - view_height - 1) / view_height + 1;

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

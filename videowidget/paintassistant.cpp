/*

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

#include "paintassistant.h"
const int PaintAssistant::repaintTime = 50;
PaintTextProperties* PaintAssistant::textProperties = NULL;

PaintAssistant::PaintAssistant(): printType(NOTHING)
{
    repaintTimer = new QTimer(this);
    //connect(repaintTimer, SIGNAL(timeout()), this, SLOT(animate()));
}

PaintAssistant::~PaintAssistant()
{
    repaintTimer->stop();
    delete repaintTimer;
}

void PaintAssistant::setPrintType(PrintType _printType)
{
    printType = _printType;
    if(printType == NOSIGNAL || printType == TEXT || printType == TITLE)
    {
           // repaintTimer->start();
    }
    else
    {
          //  repaintTimer->stop();
    }

}

void PaintAssistant::animate()
{
    repaint();
}

void PaintAssistant::paintEvent(QPaintEvent* e)
{
    printSelected(e);
}

void PaintAssistant::printSelected(QPaintEvent* e)
{
    QFrame::paintEvent(e);
    switch(printType)
    {
    case NOTHING:
        break;
    case BLANK:
        printBlank();
        break;
    case VIDEO:
        printVideoSet();
        break;
    case NOSIGNAL:
        printNoSignal();
        break;
    case TITLE:
        printTitle();
        break;
    case TEXT:
        printText();
        break;
    }
}

void PaintAssistant::printVideoSet()
{
    QPainter* painter = getPainter();
    painter->begin(this);
    paintVideoFrame();
    paintTitle();
    paintText();
    painter->end();
}

void PaintAssistant::printBlank()
{
    QPainter* painter = getPainter();
    painter->begin(this);
    painter->end();
}

void PaintAssistant::printNoSignal()
{
    QPainter* painter = getPainter();
    painter->begin(this);
    paintNoSignal();
    painter->end();
}

void PaintAssistant::paintNoSignal()
{
    QPainter* painter = getPainter();
    painter->fillRect(0, 0, this->width(), this->height(), Qt::black);
    painter->setFont(QFont("Arial", 10));
    painter->setPen(Qt::white);
    painter->drawText(rect(), Qt::AlignCenter, tr("NO SIGNAL"));
}

void PaintAssistant::printTitle()
{
    QPainter* painter = getPainter();
    painter->begin(this);
    paintTitle();
    painter->end();
}

void PaintAssistant::printText()
{
    QPainter* painter = getPainter();
    painter->begin(this);
    paintText();
    painter->end();
}


void PaintAssistant::paintTitle()
{
    QPainter* painter = getPainter();
    painter->setFont(QFont("Arial", 10));
    painter->setPen(Qt::white);
    painter->drawText(getXAssistDisplacement(), getYAssistDisplacement(), this->width() - 2*getXAssistDisplacement(),this->height() - 2*getYAssistDisplacement(), Qt::AlignBottom, getTitle());
}

int PaintAssistant::getXAssistDisplacement()
{
    return 0;
}

int PaintAssistant::getYAssistDisplacement()
{
    return 0;
}

void PaintAssistant::setTextProperties(PaintTextProperties* _textProperties)
{
    textProperties = _textProperties;
}

void PaintAssistant::paintText()
{
    QPainter* painter = getPainter();
    if(isNeedPaintText() && getPaintText().length()>0)
    {
        int size = textProperties->size + (this->height()-textProperties->heightForNoCoef)*textProperties->coefficient;
        QFont font(textProperties->font);
        QString printString = getPaintText();

        painter->fillRect(0, 0, this->width(), this->height(),QColor(0,0,0,100));
        font.setPixelSize(size);
        painter->setFont(font);
        painter->drawText(rect(), Qt::AlignTop | Qt::TextDontClip, printString);
    }
}

bool PaintAssistant::isNeedPaintText()
{
    return false;
}

QString PaintAssistant::getPaintText()
{
    return QString("");
}

QString PaintAssistant::getTitle()
{
    return QString("");
}

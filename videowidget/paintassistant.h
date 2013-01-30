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

#ifndef PAINTASSISTANT_H
#define PAINTASSISTANT_H
#include <QString>
#include <QPainter>
#include <QWidget>
#include <QFrame>
#include <QTimer>
#include "painttextproperties.h"

class PaintAssistant: public QFrame
{
public:

    PaintAssistant();
    ~PaintAssistant();

    enum PrintType {NOTHING, VIDEO, BLANK, NOSIGNAL, TITLE, TEXT}; //TITLE AND TEXT only for test
    void setPrintType(PrintType _printType);

    void printNothing();
    void printBlank();
    void printNoSignal();

    void printTitle();
    void printText();
    void printVideoSet();

    static void setTextProperties(PaintTextProperties* textProperties);
protected:
    void paintTitle();
    void paintText();
    void paintNoSignal();

    virtual bool isNeedPaintText();
    virtual QString getPaintText();
    virtual QString getTitle();
protected:
    virtual QPainter* getPainter() = 0;
    virtual void paintVideoFrame() = 0;

    virtual int getXAssistDisplacement();
    virtual int getYAssistDisplacement();

    void printSelected();

    PrintType printType;
private:
    static PaintTextProperties* textProperties;

    QTimer* repaintTimer;
    static const int repaintTime;

    void animate();
    void paintEvent(QPaintEvent*);
};

#endif // PAINTASSISTANT_H

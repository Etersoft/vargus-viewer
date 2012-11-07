#ifndef PAINTTEXTPROPERTIES_H
#define PAINTTEXTPROPERTIES_H

#include "painttextproperties.h"

class PaintTextProperties
{
public:
    PaintTextProperties();
    PaintTextProperties(QString _font, int _size, qreal _coefficient);

public:
    int size;
    qreal coefficient;
    QString font;
};

#endif // PAINTTEXTPROPERTIES_H

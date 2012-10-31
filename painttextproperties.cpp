#include <QString>
#include "painttextproperties.h"


PaintTextProperties::PaintTextProperties()
{
    font = "Courier New";
    delimeter = 20;
    coefficientx = 1.6;
    coefficienty = 0.73;
    minsize = 10;
}

PaintTextProperties::PaintTextProperties(QString _font, int _size, int _minsize, qreal _coefficient)
{
    font = _font;
    delimeter = 400/_size;
    coefficientx = _coefficient * 1.6;
    coefficienty = _coefficient * 0.73;
    minsize = _minsize;
}

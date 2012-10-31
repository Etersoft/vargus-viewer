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
    /*font = _font;
    delimeter = (int)(640/_size);
    minsize = 10;
    coefficientx = _coefficient;
    coefficienty = _coefficient/1.88;*/
    font = "Courier New";
    delimeter = 20;
    coefficientx = 1.6;
    coefficienty = 0.73;
    minsize = 10;
}

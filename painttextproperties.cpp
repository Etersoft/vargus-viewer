#include <QString>
#include "painttextproperties.h"


PaintTextProperties::PaintTextProperties()
{
    font = "Courier New";
    size = 20;
    coefficient = 0;
}

PaintTextProperties::PaintTextProperties(QString _font, int _size, qreal _coefficient):
    font(_font), size(_size), coefficient(_coefficient)
{

}

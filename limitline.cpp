#include "limitline.h"

RunningTextSettings::RunningTextSettings()
{
    color = 0xFFFFFF;
    opacity = 255;
    position = -1;
    refresh = 1000;
    size = -1;
    timeout = 0;
    x = 0;
    y = 0;
    limitLine = 13;
}

LimitLine::LimitLine(int _numLimitLine) : numLimitLine(_numLimitLine)
{
    strings = new QStringList();
}

LimitLine::~LimitLine()
{
    delete strings;
}

void LimitLine::AddString(QString string)
{
    strings->append(string);
    if(strings->count() > numLimitLine)
    {
        strings->pop_front();
    }
}

QString LimitLine::getLimitLine()
{
    QString finalResult = QString("");
    foreach(const QString &sringElement, *strings)
    {
        finalResult += sringElement + "";
    }
    return finalResult;
}

QString LimitLine::AddStringGetLine(QString* string)
{
    if(string)
        AddString(*string);
    return getLimitLine();
}

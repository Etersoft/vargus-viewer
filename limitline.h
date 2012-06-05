#ifndef LIMITLINE_H
#define LIMITLINE_H
#include <QStringList>

struct RunningTextSettings
{
    RunningTextSettings();
    int color;
    int opacity;
    int position;
    int refresh;
    int size;
    int timeout;
    int x;
    int y;
    int limitLine;
};

class LimitLine
{
private:
    QStringList* strings;
    int numLimitLine;
public:
    LimitLine(int _numLimitLine);
    ~LimitLine();
    void setNumLimitLine(int num);
    QString getLimitLine();
    void AddString(QString string);
    QString AddStringGetLine(QString* string);
};

#endif // LIMITLINE_H

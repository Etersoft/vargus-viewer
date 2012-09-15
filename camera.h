#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include "runningstring.h"
#include "limitline.h"

class Camera : public QObject, public AppendRunningString
{
    Q_OBJECT

    QString cam_name;
    QString cam_description;
    QString cam_source;
    QString cam_preview;
    QString cam_agent;
    int cameraNum;

public:
    explicit Camera(const QString &cam);
    ~Camera();
    Camera * copy();

    QString name() {return cam_name;}
    QString description() {return cam_description;}
    QString source() {return cam_source;}
    QString preview() {return cam_preview;}
    QString agent() {return cam_agent;}
    int number() {return cameraNum;}

    void setName(const QString &new_value);
    void setDescription(const QString &new_value){cam_description = new_value;}
    void setSource(const QString &new_value){cam_source = new_value;}
    void setPreview(const QString &new_value){cam_preview = new_value;}
    void setAgent(const QString &new_value){cam_agent = new_value;}
    void setNumber(int _newNumber){cameraNum= _newNumber;}

    static RunningTextSettings* runningTextSetting;
    static RunningString* runningString;
    LimitLine* runningText;
    bool isRunningStringActive;

private:
    static QString runningTextip;
    static int runningTextport;

    void setStaticParam();

public:
    static void setRunningTextAddress(QString ip, int port);
    static void changeTextServerSettings(const QString &_adress, int _port);

    virtual void appendString(QString str);
};

#endif // CAMERA_H

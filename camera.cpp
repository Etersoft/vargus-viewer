#include "camera.h"

RunningTextSettings *Camera::runningTextSetting = 0;
RunningString *Camera::runningString = 0;
QString Camera::runningTextip = "";
int Camera::runningTextport = 0;

void Camera::setStaticParam()
{
    if(!runningString)
    {
        runningString = new RunningString(runningTextip, runningTextport);
    }
    if(!runningTextSetting)
    {
        runningTextSetting = new RunningTextSettings();
    }
}

Camera::~Camera()
{
    runningString->dropAppendMethod(cam_name);
}

Camera::Camera(const QString &cam)
{
    isRunningStringActive = true;
    setStaticParam();
    setName(cam);
    runningText = new LimitLine(runningTextSetting->limitLine) ;
    cameraNum = 0;
}

Camera* Camera::copy()
{
    Camera *res = new Camera(cam_name);
    res->cam_description = cam_description;
    res->cam_source = cam_source;
    res->cam_preview = cam_preview;
    res->cam_agent = cam_agent;
    res->cameraNum = cameraNum;
    return res;
}

void Camera::setName(const QString &new_value)
{
    runningString->dropAppendMethod(cam_name);
    cam_name = new_value;
    runningString->addAppendMethod(cam_name, this);
}

void Camera::changeTextServerSettings(const QString &_adress, int _port)
{
    if(runningString)
        runningString->changeConnection(_adress, _port);
    else
        runningString = new RunningString(_adress, _port);
}

void Camera::setRunningTextAddress(QString ip, int port)
{
    runningTextip = ip;
    runningTextport = port;
}

void Camera::appendString(QString str)
{
    runningText->AddString(str);
}

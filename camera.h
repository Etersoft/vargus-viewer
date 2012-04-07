#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>

class Camera : public QObject
{
    Q_OBJECT

    QString cam_name;
    QString cam_description;
    QString cam_source;
    QString cam_preview;
    QString cam_agent;

public:
    explicit Camera(){}
    Camera(QString cam){cam_name = cam;}
    Camera * copy();

    QString name() {return cam_name;}
    QString description() {return cam_description;}
    QString source() {return cam_source;}
    QString preview() {return cam_preview;}
    QString agent() {return cam_agent;}

    void setName(QString new_value){cam_name = new_value;}
    void setDescription(QString new_value){cam_description = new_value;}
    void setSource(QString new_value){cam_source = new_value;}
    void setPreview(QString new_value){cam_preview = new_value;}
    void setAgent(QString new_value){cam_agent = new_value;}

signals:

public slots:

};

#endif // CAMERA_H

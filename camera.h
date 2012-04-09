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
    Camera(const QString &cam){cam_name = cam;}
    Camera * copy();

    QString name() {return cam_name;}
    QString description() {return cam_description;}
    QString source() {return cam_source;}
    QString preview() {return cam_preview;}
    QString agent() {return cam_agent;}

    void setName(const QString &new_value){cam_name = new_value;}
    void setDescription(const QString &new_value){cam_description = new_value;}
    void setSource(const QString &new_value){cam_source = new_value;}
    void setPreview(const QString &new_value){cam_preview = new_value;}
    void setAgent(const QString &new_value){cam_agent = new_value;}

signals:

public slots:

};

#endif // CAMERA_H

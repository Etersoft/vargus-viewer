#ifndef FPSCOUNTER_H
#define FPSCOUNTER_H

#include <QDialog>
#include <videowidget.h>
#include <QTableWidget>
#include<QGridLayout>
#include <QPushButton>

class FPSCounter : public QDialog
{
    Q_OBJECT
    QList<VideoWidget *> video;
    QTableWidget *table;
    QGridLayout *grid;
    QPushButton *updateButton;

public:
    explicit FPSCounter(QWidget *parent = 0);
    void setVideoList(const QList<VideoWidget *> &videoList) {video = videoList;}
    void start();
signals:
private slots:
    void update();
    
public slots:
    
};

#endif // FPSCOUNTER_H

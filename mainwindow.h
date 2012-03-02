#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>

#include <phonon/VideoPlayer>

#include <camera.h>
#include <set.h>
#include <view.h>
#include <QAbstractSocket>

#define MAX_PLAYERS 16

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QAbstractSocket *socket;
    QList<Camera *> camerasList;
    QList<Set *> setsList;
    QList<View *> viewsList;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    void initData();
    QString readAnswer();

};

#endif // MAINWINDOW_H

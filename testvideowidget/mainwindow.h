#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include "../videowidget.h"
#include "../camera.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    VideoWidget* vw1;
    VideoWidget* vw2;
    VideoWidget* vw3;
    VideoWidget* vw4;

    QFile* file4TestWriteTextEvent;
    QTimer* timer4Event;

public slots:
    void openArhive();
    void time2AddString();
    void showBigSize();
private slots:
    void on_pushButton_clicked();
};

#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../videowidget.h"
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

public slots:
    void openArhive();
};

#endif // MAINWINDOW_H

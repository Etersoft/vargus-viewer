#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../runningstring/runningstring.h"


namespace Ui {
    class MainWindow;
}




class MainWindow : public QMainWindow, public PrintRunningString
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    RunningString* rs;

    void printString(QString rString);
};

#endif // MAINWINDOW_H

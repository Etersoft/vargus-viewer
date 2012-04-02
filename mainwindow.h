#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include<QListWidget>

#include <phonon/VideoPlayer>
#include<QLayout>

#include <camera.h>
#include <set.h>
#include <view.h>
#include <QAbstractSocket>
#include<QMessageBox>
#include<QCloseEvent>
#include "cameralist.h"
#include "logger.h"

#define MAX_PLAYERS 16
class MainWindow : public QMainWindow
{
    Q_OBJECT

    QAbstractSocket *socket;
    QList<Camera *> camerasList;
    QList<Set *> setsList;
    QList<View *> viewsList;
    QList<Camera *> currentCameras;

    QAction *exitAction;
    QAction *aboutAction;
    QAction *delLogFilesAction;
    QAction *enableLog;

    QMenu *fileMenu;
    QMenu *helpMenu;
    QMenu *settingsMenu;

    QTabWidget *setTab;

    CameraList *camList;

    QHBoxLayout *centralLayout;
    QVBoxLayout *videoLayout;
    QVBoxLayout *controlLayout;
    QGridLayout *viewLayout;
    QHBoxLayout *buttonLayout;

    QPushButton *prevButton;
    QPushButton *resetButton;
    QPushButton *nextButton;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void initData();
    QString readAnswer();
    void createMenus();
    void createActions();
    bool okToContinue();
    void makeButtons();
    void makeSets();
    void initCameras();
    void initSets();
    void initViews();
    void createLayouts();

private slots:
    void onSetChanged(int num);
    void changeActiveCameras(QList<Camera*> activeCameras);
    void about();
    void nextGroup();
    void prevGroup();
    void resetGroup();
    void makeBigVideo(QListWidgetItem*);
    void deleteLogFiles();
    void enableLogging(bool enable);
protected:
    void closeEvent(QCloseEvent *);

};

#endif // MAINWINDOW_H

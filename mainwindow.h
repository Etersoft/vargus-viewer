#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include<QListWidget>

#include<QLayout>

#include <camera.h>
#include <set.h>
#include <view.h>
#include <QAbstractSocket>
#include<QMessageBox>
#include<QCloseEvent>
#include<QSettings>
#include "cameralist.h"
#include<QSystemTrayIcon>
#include<videosettingsdialog.h>

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
    QAction *connectionSettings;
    QAction *fpsCounterAction;
    QAction *loggingPathAction;
    QAction *defaultPathForLogs;
    QAction *videoSettingsAction;

    QMenu *fileMenu;
    QMenu *helpMenu;
    QMenu *settingsMenu;
    QMenu *contextMenu;

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

    QSettings *settings;
    QString t_server;
    QString server;

    int port;
    int t_port;
    QString pathForLogs;
    bool loggingEnabled;
    VPlayingType pltp;
    QSystemTrayIcon *trIcon;


public:
    explicit MainWindow(QWidget *parent = 0, QString server = 0, int portNum = 0, bool logging = false);
    ~MainWindow();

private:
    bool initData();
    QStringList readAnswer(int amountOfLines = 1);
    void createMenus();
    void createActions();
    void createIcons();
    bool okToContinue();
    void makeButtons();
    void makeSets();
    void initCameras();
    void initSets();
    void initViews();
    void createLayouts();
    bool readSettings();
    void setDefaultsSettings();
    void saveSettings();
    void startConnection();

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
    void changeConnectionSettings();
    void newSettings(QString newServer, int newPort, QString new_t_server, int new_t_port);
    void enableButtons(bool prev, bool next);
    void showHide(QSystemTrayIcon::ActivationReason);
    void changeLoggingFolder();
    void defaultLoggingFolder();
    void changeVideoSettings();
    void changePlayingType(VPlayingType t);
protected:
    void closeEvent(QCloseEvent *);

};

#endif // MAINWINDOW_H

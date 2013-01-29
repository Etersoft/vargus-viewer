/*
Main window.

Copyright (C) 2012-2013 Etersoft <info@etersoft.ru>

This file is part of VargusViewer.

VargusViewer is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

VargusViewer is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QListWidget>

#include <QLayout>

#include <camera.h>
#include <set.h>
#include <view.h>
#include <QAbstractSocket>
#include <QMessageBox>
#include <QCloseEvent>
#include <QSettings>
#include "cameralist.h"
#include <QSystemTrayIcon>
#include <vlcsettingsdialog.h>

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
    QAction *connectionSettings;
    QAction *vlcsettingsAction;
    QAction *logAction;
    QAction *fontsettingsAction;

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
    QString vlcSettings;

    int port;
    int t_port;
    QString pathForLogs;
    bool loggingEnabled;
    int set;
    int view;
    int fontsize;
    double magnification;
    QString fonttype;
    QSystemTrayIcon *trIcon;

public:
    explicit MainWindow(QWidget *parent = 0, QString server = 0, int portNum = 0, bool logging = true,
                        int setNum = -1, int viewNum = -1);
    ~MainWindow();
    void updateCamera(Camera *c);

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
    void changeConnectionSettings();
    void newSettings(QString newServer, int newPort, QString new_t_server, int new_t_port);
    void enableButtons(bool prev, bool next);
    void showHide(QSystemTrayIcon::ActivationReason);
    void vlcsettingsDialog();
    void newSettingsForVLC(QString &_vlcsettings);
    void logSettings();
    void fontSettings();
    void fontChanged(int newfsize, double newmagnification, QString newfont);

protected:
    void closeEvent(QCloseEvent *);
};

#endif // MAINWINDOW_H

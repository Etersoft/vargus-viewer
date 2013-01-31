#-------------------------------------------------
#
# Project created by QtCreator 2012-02-14T09:17:43
#
#-------------------------------------------------

QT       += core gui network

QMAKE_CXXFLAGS += -O2 -Wall 
unix: QMAKE_CXXFLAGS +=-Wextra -Winit-self

TARGET = VargusViewer
TEMPLATE = app

SOURCES += main.cpp\
    mainwindow.cpp \
    camera.cpp \
    set.cpp \
    view.cpp \
    cameralist.cpp \
    logger.cpp \
    logsettings.cpp \
    settingsdialog.cpp \
    vlcsettingsdialog.cpp \
    fontsettings.cpp \
    videowidget/vlccallbacks.cpp \
    videowidget/vlcpainting.cpp \
    videowidget/vlccontrol.cpp \
    videowidget/paintassistant.cpp \
    videowidget/painttextproperties.cpp \
    videowidget/videomath.cpp \
    videowidget/videowidget.cpp \
    runningstring/runningstring.cpp \
    runningstring/worksock.cpp \
    runningstring/limitline.cpp 

HEADERS  += mainwindow.h \
    set.h \
    view.h \
    camera.h \
    cameralist.h \
    logger.h \
    logsettings.h \
    settingsdialog.h \
    vlcsettingsdialog.h \
    fontsettings.h \
    videowidget/vlccallbacks.h \
    videowidget/vlcpainting.h \
    videowidget/vlccontrol.h \
    videowidget/paintassistant.h \
    videowidget/painttextproperties.h \
    videowidget/videomath.h \
    videowidget/videowidget.h \
    runningstring/runningstring.h \
    runningstring/worksock.h \
    runningstring/limitline.h 

TRANSLATIONS += lang/lang_ru.ts

unix :  DEFINES += DATADIR=\\\"/usr/share/vargus-viewer/\\\"
win32 :    DEFINES += DATADIR=\\\"./\\\"
unix :  DEFINES += WORKDIR=\\\"$(HOME)/vargus-viewer/\\\"
win32 :    DEFINES += WORKDIR=\\\"./log/\\\"


unix: LIBS += -lvlc
win32: LIBS += -L"C:/Program Files (x86)/VideoLan/VLC/sdk/lib" -llibvlc
win32: INCLUDEPATH += "C:/Program Files (x86)/VideoLan/VLC/sdk/include"
win32: DEPENDPATH += sdk
FORMS +=


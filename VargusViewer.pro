#-------------------------------------------------
#
# Project created by QtCreator 2012-02-14T09:17:43
#
#-------------------------------------------------

QT       += core gui network

TARGET = VargusViewer
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    camera.cpp \
    set.cpp \
    view.cpp \
    videowidget.cpp \
    cameralist.cpp \
    logger.cpp \
    settingsdialog.cpp \
    runningstring.cpp \
    worksock.cpp \
    videosettingsdialog.cpp \
    videowidgetlowlevelpainting.cpp \
    limitline.cpp \
    videowidgetdeleter.cpp \
    vlcsettingsdialog.cpp

HEADERS  += mainwindow.h \
    set.h \
    view.h \
    videowidget.h \
    camera.h \
    cameralist.h \
    logger.h \
    settingsdialog.h \
    runningstring.h \
    worksock.h \
    videosettingsdialog.h \
    enums.h \
    videowidgetlowlevelpainting.h \
    limitline.h \
    videowidgetdeleter.h \
    vlcsettingsdialog.h

TRANSLATIONS += lang/lang_ru.ts

DEFINES += DATADIR=\\\"/usr/share/vargus-viewer/\\\"
unix {
   DEFINES += WORKDIR=\\\"$(HOME)/vargus-viewer/\\\"
}
!unix {
   DEFINES += WORKDIR=\\\".\\\"
}

unix|win32: LIBS += -lvlc

FORMS +=

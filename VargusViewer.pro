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
    fpscounter.cpp

HEADERS  += mainwindow.h \
    set.h \
    view.h \
    videowidget.h \
    camera.h \
    cameralist.h \
    logger.h \
    settingsdialog.h \
    fpscounter.h

FORMS    +=

TRANSLATIONS += langs/lang_ru.ts

DEFINES += DATADIR=\\\"/usr/share/vargus-viewer/\\\"

unix|win32: LIBS += -lvlc

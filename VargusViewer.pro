#-------------------------------------------------
#
# Project created by QtCreator 2012-02-14T09:17:43
#
#-------------------------------------------------

QT       += core gui phonon network

# FIXME: absolute path to headers
INCLUDEPATH += /usr/include/kde4

TARGET = VargusViewer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    camera.cpp \
    set.cpp

HEADERS  += mainwindow.h \
    camera.h \
    set.h

FORMS    += mainwindow.ui

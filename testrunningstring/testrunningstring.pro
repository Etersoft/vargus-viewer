#-------------------------------------------------
#
# Project created by QtCreator 2012-04-13T18:00:56
#
#-------------------------------------------------

QT       += core gui network

TARGET = testrunningstring
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ../runningstring/runningstring.cpp \
    ../runningstring/worksock.cpp

HEADERS  += mainwindow.h \
    ../runningstring/runningstring.h \
    ../runningstring/worksock.h

FORMS    += mainwindow.ui

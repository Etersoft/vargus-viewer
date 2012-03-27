#-------------------------------------------------
#
# Project created by QtCreator 2012-03-14T13:00:21
#
#-------------------------------------------------

QT       += core gui

TARGET = testvideowidget
TEMPLATE = app
LIBS += /usr/lib64/libvlc.so
LIBS += -L/usr/lib64/libvlc/

SOURCES += main.cpp\
        mainwindow.cpp \
    ../videowidget.cpp \
    dragtextedit.cpp \
    ../camera.cpp

HEADERS  += mainwindow.h \
    ../videowidget.h \
    dragtextedit.h \
    ../camera.h

unix|win32: LIBS += -lvlc

FORMS    += mainwindow.ui

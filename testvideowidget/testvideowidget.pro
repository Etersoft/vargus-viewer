#-------------------------------------------------
#
# Project created by QtCreator 2012-03-14T13:00:21
#
#-------------------------------------------------

QT       += core gui network
QMAKE_CXXFLAGS += -O2 -Wall


TARGET = testvideowidget
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        ../logger.cpp \
        ../videowidget/vlccallbacks.cpp \
        ../videowidget/vlcpainting.cpp \
        ../videowidget/vlccontrol.cpp \
        ../videowidget/paintassistant.cpp \
        ../videowidget/painttextproperties.cpp \
        ../videowidget/videomath.cpp \
        ../videowidget/videowidget.cpp \
        ../videowidget/videowidgetdragdata.cpp \
        ../runningstring/runningstring.cpp \
        ../runningstring/worksock.cpp \
        ../runningstring/limitline.cpp \
        dragtextedit.cpp \
        ../camera.cpp

HEADERS  += mainwindow.h \
            ../logger.h \
            ../videowidget/vlccallbacks.h \
            ../videowidget/vlcpainting.h \
            ../videowidget/vlccontrol.h \
            ../videowidget/paintassistant.h \
            ../videowidget/painttextproperties.h \
            ../videowidget/videomath.h \
            ../videowidget/videowidget.h \
            ../videowidget/videowidgetdragdata.h \
            ../runningstring/runningstring.h \
            ../runningstring/worksock.h \
            ../runningstring/limitline.h \
            dragtextedit.h \
            ../camera.h

unix :  DEFINES += DATADIR=\\\"/usr/share/vargus-viewer/\\\"
win32 :    DEFINES += DATADIR=\\\"./\\\"
unix :  DEFINES += WORKDIR=\\\"$(HOME)/vargus-viewer/\\\"
win32 :    DEFINES += WORKDIR=\\\"./\\\"


unix: LIBS += -lvlc
!exists( cross-compile ) {
    win32: LIBS += -L"C:/Program Files (x86)/VideoLan/VLC/sdk/lib" -llibvlc
    win32: INCLUDEPATH += "C:/Program Files (x86)/VideoLan/VLC/sdk/include"
    win32: DEPENDPATH += sdk
}
exists( cross-compile ) {
    win32: LIBS += -L"/home/guest/Projects/VideoLan/VLC/sdk/lib" -llibvlc
    win32: INCLUDEPATH += "/home/guest/Projects/VideoLan/VLC/sdk/include"
    win32: DEPENDPATH += sdk
}
FORMS    += mainwindow.ui

#-------------------------------------------------
#
# Project created by QtCreator 2012-03-14T13:00:21
#
#-------------------------------------------------

QT       += core gui network
QMAKE_CXXFLAGS += -O2 -Wall


TARGET = testvideowidget
TEMPLATE = app
LIBS += -L"C:/Program Files (x86)/VideoLan/VLC/sdk/lib" -llibvlc


INCLUDEPATH += "C:/Program Files (x86)/VideoLan/VLC/sdk/include"


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

unix|win32: LIBS += -lvlc
DEFINES += WORKDIR=\\\"./\\\"
DEFINES += DATADIR=\\\"./\\\"
FORMS    += mainwindow.ui

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

#include <QApplication>
#include <QTextCodec>
#include <QTranslator>
#include <QString>
#include <QLibraryInfo>
#include "mainwindow.h"
#include <signal.h>
#include "logger.h"

#ifndef WIN32
    #include <execinfo.h>
    #include <unistd.h>
#endif /*WIN32*/

Logger &vargusLog = Logger::instance();

void signal_handler(int sig)// signal handler
{
    switch(sig)
    {
#ifndef WIN32
        case SIGHUP:
            vargusLog.writeToFile("Signal SIGHUP");
            break;
        case SIGTERM:
            vargusLog.writeToFile("Signal SIGTERM");
            break;
        case SIGABRT:
            vargusLog.writeToFile("Signal SIGABRT");
            break;
        case SIGKILL:
            vargusLog.writeToFile("Signal SIGKILL");
            break;
#endif /*WIN32*/
        default:
            vargusLog.writeToFile("Signal id" + QString::number(sig));
    }
    fprintf(stderr,"Error");
    fflush(stderr);
    const int max_deep = 100;
    void  *traces[max_deep];
#ifndef WIN32
    int ntprs = backtrace(traces, max_deep);
    backtrace_symbols_fd(traces, ntprs, STDERR_FILENO);
#endif /*WIN32*/
    exit(sig);
}

//print information about command line options
void usage()
{
    printf("VargusViewer -server xxx.xxx.xxx.xxx -port xxxx -disablelogging -set x -view x\n");
    printf("-server - server adress\n");
    printf("-port - port number\n");
    printf("-disablelogging - disable writing logs. By default logging is enabled.\n");
    printf("-set - number of set to show after program started. \"Set\" must be >=0 and < amount os sets. If it is wrong, \"set\" will be 0.\n");
    printf("-view - number of view to show after program started. \"View\" must be >=0 and < amount os views. If it is wrong, \"view\" will be 0.\n");
}

//print "error" line
void argsError()
{
    printf("Args error\n");
    usage();
    exit(10);
}

void processCommandLine(int argc, QStringList args, QString &serv, int &port, bool &logging, int &set, int &view)
{
    int size = argc-1;
    for(int i = 0; i < args.size();)
    {
        if(args.at(i) == QString("-help"))
        {
            usage();
            exit(0);
        }
        else if(args.at(i) == QString("-server"))
        {
            if((size - i) <= 1)
            {
                argsError();
            }
            serv = args.at(i+1);
            i=i+2;
        }
        else if(args.at(i) == QString("-port"))
        {
            if((size - i) <= 1)
            {
                argsError();
            }
            port = args.at(i+1).toInt();
            i=i+2;
        }
        else if(args.at(i) == QString("-disablelogging"))
        {
                logging = false;
                i++;
        }
        else if(args.at(i) == QString("-set"))
        {
            if((size - i) <= 1)
            {
                argsError();
            }
            set = args.at(i+1).toInt();
            i=i+2;
        }
        else if(args.at(i) == QString("-view"))
        {
            if((size - i) <= 1)
            {
                argsError();
            }
            view = args.at(i+1).toInt();
            i=i+2;
        }
        else
            argsError();
    }
}

int main(int argc, char *argv[])
{
    vargusLog.makeLogFile();
    vargusLog.writeToFile("Start vargusLog");
#ifndef WIN32
    //set signal's hanler
    signal(SIGKILL, signal_handler);
    signal(SIGHUP, signal_handler);
    signal(SIGABRT, signal_handler);
    signal(SIGTERM, signal_handler);
#endif /*WIN32*/
    QApplication app(argc, argv);
    QString imagePath = "/usr/share/vargus-viewer/images/";
    QIcon icon(imagePath + "vargus_big.png");
    QApplication::setWindowIcon(icon);
    QTextCodec::setCodecForTr( QTextCodec::codecForName("utf8") );

    //set translator
    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&qtTranslator);

    QTranslator *translator = new QTranslator(&app);
    if (translator->load("lang_" + QLocale::system().name(), QString(DATADIR) + "lang/"))
    {
        app.installTranslator(translator);
    }

    //processing of command line
    QString serv;
    int port = 0;
    bool logging = true;
    int set = -1;
    int view = -1;
    QStringList args;
    for(int i = 1; i < argc; i++)
    {
        args.append(QString(argv[i]));
    }

    //process command line
    processCommandLine(argc, args, serv, port, logging, set, view);

    //start application
    MainWindow w(0, serv, port, logging, set, view);
    w.showMaximized();

    return app.exec();
}

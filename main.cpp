#include <QApplication>
#include <QTextCodec>
#include <QTranslator>
#include <QString>
#include <QLibraryInfo>
#include "mainwindow.h"
#include <signal.h>
#include <logger.h>
#include <execinfo.h>
#include <unistd.h>

Logger &vargusLog = Logger::instance();

void signal_handler(int sig)
{
    switch(sig)
    {
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
        default:
            vargusLog.writeToFile("Signal id" + QString::number(sig));
    }
    fprintf(stderr,"Error");
    fflush(stderr);
    const int max_deep = 100;
    void  *traces[max_deep];
    int ntprs = backtrace(traces, max_deep);
    backtrace_symbols_fd(traces, ntprs, STDERR_FILENO);
    exit(sig);
}

void usage()
{
    printf("VargusViewer -server xxx.xxx.xxx.xxx -port xxxx -disablelogging -set x -view x\n");
    printf("-server - server adress\n");
    printf("-port - port number\n");
    printf("-disablelogging - disable writing logs. By default logging is enabled.\n");
    printf("-set - number of set to show after program started. \"Set\" must be >=0 and < amount os sets. If it is wrong, \"set\" will be 0.\n");
    printf("-view - number of view to show after program started. \"View\" must be >=0 and < amount os views. If it is wrong, \"view\" will be 0.\n");
}

void argsError()
{
    printf("Args error\n");
    usage();
    exit(10);
}

int main(int argc, char *argv[])
{
    vargusLog.makeLogFile();
    vargusLog.writeToFile("Start vargusLog");
    signal(SIGKILL, signal_handler);
    signal(SIGHUP, signal_handler);
    signal(SIGABRT, signal_handler);
    signal(SIGTERM, signal_handler);
    QApplication app(argc, argv);
    QString imagePath = "/usr/share/vargus-viewer/images/";
    QIcon icon(imagePath + "vargus_big.png");
    QApplication::setWindowIcon(icon);
    QTextCodec::setCodecForTr( QTextCodec::codecForName("utf8") );

    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&qtTranslator);

    QTranslator *translator = new QTranslator(&app);
    if (translator->load("lang_" + QLocale::system().name(), QString(DATADIR) + "lang/"))
    {
        app.installTranslator(translator);
    }

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

    MainWindow w(0,serv,port,logging,set,view);
    w.showMaximized();

    return app.exec();
}

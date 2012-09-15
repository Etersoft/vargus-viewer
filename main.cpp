#include <QApplication>
#include <QTextCodec>
#include <QTranslator>
#include <QString>
#include <QLibraryInfo>
#include "mainwindow.h"
#include<signal.h>
#include<logger.h>

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
    exit(sig);
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
    if(argc >= 2)
        serv = argv[1];
    if(argc >= 3)
    {
        QString p(argv[2]);
        port = p.toInt();
    }
    if(argc >= 4)
    {
        QString p(argv[3]);
        int l = p.toInt();
        if(l == 0)
            logging = false;
    }

    MainWindow w(0,serv,port,logging);
    w.show();

    return app.exec();
}

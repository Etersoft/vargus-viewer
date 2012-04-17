#include <QApplication>
#include <QTextCodec>
#include <QTranslator>
#include <QString>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QString imagePath = "/usr/share/vargus-viewer/images/";
    QIcon icon(imagePath + "vargus_big.png");
    QApplication::setWindowIcon(icon);
    QTextCodec::setCodecForTr( QTextCodec::codecForName("utf8") );
    QString translatorFileName = QLatin1String("qt_ru.qm");
    QTranslator *translator = new QTranslator(&app);
    if (translator->load(translatorFileName,"/usr/lib64/qt3/translations/"))
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

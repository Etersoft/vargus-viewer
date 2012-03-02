#include <QApplication>
#include <QTextCodec>
#include <QTranslator>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QTextCodec::setCodecForTr( QTextCodec::codecForName("utf8") );
    QString translatorFileName = QLatin1String("qt_ru");
    QTranslator *translator = new QTranslator(&app);
    if (translator->load(translatorFileName, "./"))
    {
        app.installTranslator(translator);
    }

    MainWindow w;
    w.show();

    return app.exec();
}

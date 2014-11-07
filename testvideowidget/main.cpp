#include <QtGui/QApplication>
#include "mainwindow.h"
#include "../logger.h"
Logger &vargusLog = Logger::instance();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}

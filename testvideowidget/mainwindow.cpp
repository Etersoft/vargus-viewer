#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QProcess>
#include <QDesktopServices>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    vw1 = new VideoWidget();
    vw1->setUrlVideoStream(QUrl("1.avi"),VideoWidget::SMALLVIDEO);
    vw1->startPlay(VideoWidget::SMALLVIDEO);
    ui->gridLayout->addWidget(vw1,0,0);

    connect(vw1,SIGNAL(arhiveCall()),this,SLOT(openArhive()));

    vw2 = new VideoWidget();
    vw2->setUrlVideoStream (QUrl("2.avi"),VideoWidget::SMALLVIDEO);
    ui->gridLayout_2->addWidget(vw2);
    vw2->startPlay(VideoWidget::SMALLVIDEO);

    vw3 = new VideoWidget();
    ui->gridLayout_3->addWidget(vw3);

    vw4 = new VideoWidget();
    ui->gridLayout_4->addWidget(vw4);

    ui->lineEdit->setText("1.avi");
    ui->lineEdit_2->setText("2.avi");

    file4TestWriteTextEvent = new QFile("1.txt");
    if (file4TestWriteTextEvent->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        timer4Event = new QTimer();
        connect(timer4Event, SIGNAL(timeout()), this, SLOT(time2AddString()));
        timer4Event->start(3000);
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openArhive()
{
    QDesktopServices::openUrl(QUrl("http://google.com"));

}

void MainWindow::time2AddString()
{
    QString readingString = file4TestWriteTextEvent->readLine();
    vw1->writeTextString(readingString.remove(QChar('\n'),Qt::CaseInsensitive));
}


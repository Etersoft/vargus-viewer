#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QProcess>
#include <QDesktopServices>
#include <QTimer>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    Camera* cam1 = new Camera("Name");
    cam1->setDescription("Desription");
    cam1->setPreview("1.avi");
    cam1->setSource("1.avi");

    ui->setupUi(this);

    vw1 = new VideoWidget();
    vw1->setCamera(cam1);
    vw1->startPlay(VideoWidget::SMALLVIDEO);
    ui->gridLayout->addWidget(vw1,0,0);

    connect(vw1,SIGNAL(arhiveCall()),this,SLOT(openArhive()));
    connect(vw1,SIGNAL(bigSizeCall()),this,SLOT(showBigSize()));

    Camera* cam2 = new Camera("Name");
    cam2->setDescription("Desription");
    cam2->setPreview("2.avi");
    cam2->setSource("2.avi");


    vw2 = new VideoWidget();
    ui->gridLayout_2->addWidget(vw2);
    vw2->setCamera(cam2);
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
    if(vw1)
        delete vw1;
    if(vw2)
        delete vw2;
    if(vw3)
        delete vw3;
    if(vw4)
        delete vw4;
    delete ui;
}

void MainWindow::openArhive()
{
    QDesktopServices::openUrl(QUrl("http://google.com"));

}

void MainWindow::showBigSize()
{
    QMessageBox msgBox;
    msgBox.setText("Need 2 show big size video");
    msgBox.exec();
}

void MainWindow::time2AddString()
{
    QString readingString = file4TestWriteTextEvent->readLine();
    //vw1->writeTextString(readingString.remove(QChar('\n'),Qt::CaseInsensitive));
}


void MainWindow::on_pushButton_clicked()
{
    delete vw2;
    vw2 = 0;
}

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    vw1 = new VideoWidget();
    vw1->setUrlVideoStream(QUrl("1.avi"),VideoWidget::SMALLVIDEO);
    vw1->startPlay(VideoWidget::SMALLVIDEO);
    ui->gridLayout->addWidget(vw1,0,0);

    vw2 = new VideoWidget();
    vw2->setUrlVideoStream (QUrl("2.avi"),VideoWidget::SMALLVIDEO);
    ui->gridLayout_2->addWidget(vw2);
    vw2->startPlay(VideoWidget::SMALLVIDEO);
}

MainWindow::~MainWindow()
{
    delete ui;
}

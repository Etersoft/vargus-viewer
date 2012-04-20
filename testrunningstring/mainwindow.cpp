#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    rs = new RunningString();
    rs->addPrintMethod((QString)"cam1",this);

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    rs->print2Cam(ui->lecam->text(),ui->lestring->text());
}

void MainWindow::printString(QString rString)
{
    ui->lineEdit_2->setText(rString);
}




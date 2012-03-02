#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFile>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initData();
}

void MainWindow::initData()
{
    // Чтение конфигурационного файла
    QFile configFile("vargus.cfg");
    if(!configFile.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(NULL, tr("Error"), tr("Can't open file"));
        return;
    }

    QString server;
    int port;

    QString confString = configFile.readLine();
    while(!confString.isEmpty())
    {
        if(!confString.contains('#') && confString.contains('='))
        {
            int index = confString.indexOf('=');
            QString var = confString.left(index);
            QString value = confString.mid(index + 1).trimmed();

            if(var == "SERVER")
                server = value.toUtf8();
            else if(var == "PORT")
                port = value.toInt();
        }
        confString = configFile.readLine();
    }
    configFile.close();

    // Сеанс связи с сервером
    socket = new QAbstractSocket(QAbstractSocket::TcpSocket, this);
    socket->connectToHost(server, port);
    if(!socket->waitForConnected())
        QMessageBox::critical(NULL, tr("Error"), tr("Cannot connect to server"));

    // TODO: нужно "сбрасывать" приветствие
    QMessageBox::information(NULL, tr("info"), readAnswer());

    // Инициализация камер
    socket->write("query camera;quantity\n");
    int cameras = readAnswer().trimmed().toInt();
    for(int i = 0; i < cameras; i++)
    {
        socket->write(QString("query camera;" + QString::number(i+1) + ";name\n").toAscii());
        camerasList << new Camera(readAnswer().trimmed());
        socket->write(QString("query camera;" + QString::number(i+1) + ";description\n").toAscii());
        camerasList.at(i)->setDescription(readAnswer().trimmed());
        socket->write(QString("query camera;" + QString::number(i+1) + ";source\n").toAscii());
        camerasList.at(i)->setSource(readAnswer().trimmed());
        socket->write(QString("query camera;" + QString::number(i+1) + ";preview\n").toAscii());
        camerasList.at(i)->setPreview(readAnswer().trimmed());
        socket->write(QString("query camera;" + QString::number(i+1) + ";agent\n").toAscii());
        camerasList.at(i)->setAgent(readAnswer().trimmed());
    }

    // Инициализация сетов
    socket->write("query set;quantity\n");
    int sets = readAnswer().trimmed().toInt();
    ui->tabWidget->clear();
    for(int i = 0; i < sets; i++)
    {
        socket->write(QString("query set;" + QString::number(i+1) + ";description\n").toAscii());
        setsList << new Set(readAnswer().trimmed());
        socket->write(QString("query set;" + QString::number(i+1) + ";cameras\n").toAscii());
        QStringList camlist = readAnswer().trimmed().split(',');
        for(int j = 0; j < camlist.count(); j++)
            for(int k = 0; k < camerasList.count(); k++)
            {
                if(camlist.at(j) == camerasList.at(k)->name())
                {
                    setsList.at(i)->addCamera(camerasList.at(k));
                    break;
                }
            }
        QWidget *tab = new QWidget;
        ui->tabWidget->addTab(tab, setsList.at(i)->description());
    }

    /*
    // Проверка: корректность заполнения сетов камерами
    for(int i = 0; i < setsList.count(); i++)
    {
        QStringList outList = QStringList() << setsList.at(i)->description();
        outList << setsList.at(i)->camerasNames().join(":");
        QMessageBox::information(NULL, tr("info"), outList.join("\n"));
    }
    */

    socket->write("exit\n");
    socket->disconnect();
    delete socket;
}

QString MainWindow::readAnswer()
{
    QString answer;
    for(answer.clear(); answer.isEmpty() || answer.contains(">"); socket->waitForReadyRead())
        answer = trUtf8(socket->readLine().data());
    return answer;
}

MainWindow::~MainWindow()
{
    delete ui;
}

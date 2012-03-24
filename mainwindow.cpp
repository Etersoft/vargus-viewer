#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QList>
#include<QListIterator>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    makeButtons();
    createActions();
    this->setWindowTitle(tr("VargusViewer"));
    // Обработка входных данных
    initData();

    // Заполнение вкладок-сетов
    for(int i = 0; i < setsList.count(); i ++)
    {
        Set *set = setsList.at(i);
        for(int j = 0; j < viewsList.count(); j ++)
        {
            set->addView(viewsList.at(j));
        }
        set->setActiveView(0);
        ui->tabWidget->addTab(set, set->description());
    }

    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(onSetChanged(int)));

    ui->tabWidget->setCurrentIndex(0);
    for(int i = 0; i < setsList.length(); i++)
        setsList.at(i)->makeVideoWidgets();
    onSetChanged(0);
    for(int i = 0; i < setsList.length(); i++)
        connect(setsList.at(i),SIGNAL(updateActiveCameras(QList<Camera*>)),this,SLOT(changeActiveCameras(QList<Camera*>)));
    changeActiveCameras(setsList.at(0)->getActiveCameras());

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
    if(!socket->waitForConnected(5000))
        QMessageBox::critical(NULL, tr("Error"), tr("Cannot connect to server"));

    // Инициализация камер
    socket->write("query camera;quantity\n");
    int cameras = readAnswer().trimmed().toInt();
    for(int i = 0; i < cameras; i++)
    {
        socket->write(QString("query camera;" + QString::number(i+1) + ";name\n").toAscii());
        camerasList << new Camera(readAnswer().trimmed());
        socket->write(QString("query camera;" + QString::number(i+1) + ";description\n").toAscii());
        camerasList.at(i)->setDescription(readAnswer().trimmed());
        socket->write(QString("query camera;" + QString::number(i+1) + ";view:source\n").toAscii());
        camerasList.at(i)->setSource(readAnswer().trimmed());
        socket->write(QString("query camera;" + QString::number(i+1) + ";view:preview\n").toAscii());
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
    }

    // Инициализация раскладок
    socket->write("query view;quantity\n");
    int views = readAnswer().trimmed().toInt();
    for(int i = 0; i < views; i++)
    {
        socket->write(QString("query view;" + QString::number(i+1) + ";description\n").toAscii());
        viewsList << new View(readAnswer().trimmed());
        socket->write(QString("query view;" + QString::number(i+1) + ";geometry:width\n").toAscii());
        viewsList.at(i)->setWidth(readAnswer().trimmed().toInt());
        socket->write(QString("query view;" + QString::number(i+1) + ";geometry:height\n").toAscii());
        viewsList.at(i)->setHeight(readAnswer().trimmed().toInt());
        socket->write(QString("query view;" + QString::number(i+1) + ";geometry:double\n").toAscii());
        viewsList.at(i)->setDoubleFrames(readAnswer().trimmed().split(','));
        socket->write(QString("query view;" + QString::number(i+1) + ";geometry:triple\n").toAscii());
        viewsList.at(i)->setTripleFrames(readAnswer().trimmed().split(','));
        socket->write(QString("query view;" + QString::number(i+1) + ";geometry:quadruple\n").toAscii());
        viewsList.at(i)->setQuadrupleFrames(readAnswer().trimmed().split(','));

        viewsList.at(i)->createIcons();
    }

    socket->write("exit\n");
    socket->disconnect();
    delete socket;
}

QString MainWindow::readAnswer()
{
    QString answer;
    int tries = 0;
    while(tries < 5)
    {
        if(!socket->waitForReadyRead(2000))
        {
            tries++;
            continue;
        }
        answer = trUtf8(socket->readLine().data());
        if(!answer.isEmpty() && answer.at(0) != ':' && answer.at(0) != '>')
            break;
    }

    if (tries == 5)
        QMessageBox::critical(this, tr("Error"), tr("Server is not response"));

    return answer;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onSetChanged(int num)
{
    for( int i = 0; i < setsList.length(); i++)
        if(setsList.at(i)->isActive())
        {
            setsList.at(i)->stopPlay();
            setsList.at(i)->setActive(false);
        }

    // Заполнение панели раскладок
    while(!ui->viewLayout->isEmpty())
    {
        QWidget *s = ui->viewLayout->takeAt(0)->widget();
        s->hide();
    }

    for(int i = 0; i < viewsList.count(); i ++)
    {
        View *view = setsList.at(ui->tabWidget->currentIndex())->views().at(i);
        view->show();
        ui->viewLayout->addWidget(view, i / 2, i % 2);
    }
    setsList.at(num)->setActive(true);
    setsList.at(num)->restoreState();
    //changeActiveCameras(setsList.at(num)->getActiveCameras());

}

void MainWindow::createActions()
{
    exitAction = new QAction(tr("&Exit"),this);
    connect(ui->actionExit,SIGNAL(triggered()),this,SLOT(close()));

    aboutAction = new QAction(tr("&About"),this);
    connect(ui->actionAbout,SIGNAL(triggered()),this,SLOT(about()));
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("О программе"),
             tr("<h2>VargusViewer</h2><p>Etersoft"));

}


void MainWindow::closeEvent(QCloseEvent *event)
{
    if(okToContinue())
        QMainWindow::closeEvent(event);
    else event->ignore();
}

bool MainWindow::okToContinue()
{
    int r = QMessageBox::warning(this,tr("Vargus Viewer"),tr("Вы уверены, что хотите выйти?"), QMessageBox::Yes|QMessageBox::No);
    if(r == QMessageBox::Yes)
        return true;
    else return false;
}

void MainWindow::changeActiveCameras(QList<Camera *> activeCameras)
{
    currentCameras.clear();
    for(int i = 0; i < setsList.length(); i++)
    {
        if(setsList.at(i)->isActive())
            currentCameras = setsList.at(i)->cameras();
    }
    QList<Camera *>::iterator it = currentCameras.begin();
    QList<Camera *>::iterator end = currentCameras.end();
    ui->cameraList->clear();
    int camNum =1;
    while(it != end)
    {

        ui->cameraList->addItem(QString::number(camNum++) + ". " + (*it)->description());
        it++;
    }
    QList<Camera *>::iterator itac = activeCameras.begin();
    QList<Camera *>::iterator endac = activeCameras.end();
    while(itac != endac)
    {
        int cameraNum = 0;
        it = currentCameras.begin();
        while(it != end)
        {
            if((*itac) == (*it))
            {
                QColor color(0,255,255);
                ui->cameraList->item(cameraNum)->setBackgroundColor(color);
                break;
            }
            it++;
            cameraNum++;
        }
        itac++;
    }
}

void MainWindow::makeButtons()
{
    ui->prevButton->setIcon(QIcon("images/prev.png"));
    ui->resetButton->setIcon(QIcon("images/reset.png"));
    ui->nextButton->setIcon(QIcon("images/next.png"));
    connect(ui->nextButton,SIGNAL(clicked()),this,SLOT(nextGroup()));
    connect(ui->prevButton,SIGNAL(clicked()),this,SLOT(prevGroup()));
    connect(ui->resetButton,SIGNAL(clicked()),this,SLOT(resetGroup()));
}

void MainWindow::nextGroup()
{
    QList<Set *>::iterator it = setsList.begin();
    QList<Set *>::iterator end = setsList.end();
    while(it != end)
    {
        if((*it)->isActive())
        {
            (*it)->next();
            break;
        }
        it++;
    }
}

void MainWindow::prevGroup()
{
    QList<Set *>::iterator it = setsList.begin();
    QList<Set *>::iterator end = setsList.end();
    while(it != end)
    {
        if((*it)->isActive())
        {
            (*it)->prev();
            break;
        }
        it++;
    }
}

void MainWindow::resetGroup()
{
    QList<Set *>::iterator it = setsList.begin();
    QList<Set *>::iterator end = setsList.end();
    while(it != end)
    {
        if((*it)->isActive())
        {
            (*it)->reset();
            break;
        }
        it++;
    }
}

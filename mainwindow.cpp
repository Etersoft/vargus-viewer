#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QList>
#include<QListIterator>
#include<QMessageBox>
#include<QDir>
Logger &log = Logger::instance();

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    if(!log.makeLogFile())
    {
        int n = QMessageBox::warning(this,"Warning",tr("Невозможно открыть файл для записи логов.\n Продолжить работу?"),tr("Да"),tr("Нет"),QString(),0,1);
        if(n) exit(1);
    }

    log.writeToFile("PROGRAM STARTED");
    ui->setupUi(this);
    createMenus();
    makeButtons();
    createActions();
    camList = new CameraList(this);
    ui->controlLayout->addWidget(camList);
    camList->setMaximumWidth(ui->nextButton->width()*4);
    setWindowTitle(tr("VargusViewer"));
    // Обработка входных данных
    initData();

    // Заполнение вкладок-сетов
    makeSets();

    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(onSetChanged(int)));

    ui->tabWidget->setCurrentIndex(0);
    onSetChanged(0);
    for(int i = 0; i < setsList.length(); i++)
        connect(setsList.at(i),SIGNAL(updateActiveCameras(QList<Camera*>)),this,SLOT(changeActiveCameras(QList<Camera*>)));
    changeActiveCameras(setsList.at(0)->getActiveCameras());
    connect(camList,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(makeBigVideo(QListWidgetItem*)));

}

void MainWindow::initData()
{
    log.writeToFile("Reading .cfg file");
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
    log.writeToFile(".cfg file is read");

    // Сеанс связи с сервером
    log.writeToFile("Server conection started");
    socket = new QAbstractSocket(QAbstractSocket::TcpSocket, this);
    socket->connectToHost(server, port);
    if(!socket->waitForConnected(5000))
        QMessageBox::critical(NULL, tr("Error"), tr("Cannot connect to server"));

    // Инициализация камер
    initCameras();
    // Инициализация сетов
    initSets();
    // Инициализация раскладок
    initViews();

    socket->write("exit\n");
    socket->disconnect();
    delete socket;
    log.writeToFile("Connection closed. Initialization ended");
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
    QList<Camera *>::iterator itc = camerasList.begin();
    QList<Camera *>::iterator endc = camerasList.end();
    while(itc != endc)
    {
        delete (*itc);
        itc++;
    }
    QList<View *>::iterator itv = viewsList.begin();
    QList<View *>::iterator endv = viewsList.end();
    while(itv != endv)
    {
        delete (*itv);
        itv++;
    }
    delete delLogFilesAction;
    delete exitAction;
    delete aboutAction;
    delete fileMenu;
    delete helpMenu;
    log.writeToFile("PROGRAM ENDED");
    log.closeFile();
}

void MainWindow::onSetChanged(int num)
{
    log.writeToFile("New active set " + setsList.at(num)->description());
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
}

void MainWindow::createActions()
{
    delLogFilesAction = new QAction(tr("&Удалить log файлы"),this);
    fileMenu->addAction(delLogFilesAction);
    connect(delLogFilesAction,SIGNAL(triggered()),this,SLOT(deleteLogFiles()));

    fileMenu->addSeparator();
    exitAction = new QAction(tr("&Exit"),this);
    fileMenu->addAction(exitAction);
    connect(exitAction,SIGNAL(triggered()),this,SLOT(close()));

    aboutAction = new QAction(tr("&О программе"),this);
    helpMenu->addAction(aboutAction);
    connect(aboutAction,SIGNAL(triggered()),this,SLOT(about()));
}

void MainWindow::about()
{
    log.writeToFile("Action about clicked");
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
    for(int i = 0; i < setsList.length(); i++)
    {
        if(setsList.at(i)->isActive())
            camList->setCurrentCameras(setsList.at(i)->cameras());
    }
    camList ->setActiveCameras(activeCameras);
    camList->print();
}

void MainWindow::makeButtons()
{
    log.writeToFile("Making buttons");
    ui->prevButton->setIcon(QIcon("images/prev.png"));
    ui->resetButton->setIcon(QIcon("images/reset.png"));
    ui->nextButton->setIcon(QIcon("images/next.png"));
    connect(ui->nextButton,SIGNAL(clicked()),this,SLOT(nextGroup()));
    connect(ui->prevButton,SIGNAL(clicked()),this,SLOT(prevGroup()));
    connect(ui->resetButton,SIGNAL(clicked()),this,SLOT(resetGroup()));
    log.writeToFile("Buttons made");
}

void MainWindow::nextGroup()
{
    log.writeToFile("Clicked next button");
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
    log.writeToFile("Clicked previous button");
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
    log.writeToFile("Clicked reset button");
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

void MainWindow::makeBigVideo(QListWidgetItem * item)
{
    Set *activeSet;
    QList<Set *>::iterator it = setsList.begin();
    QList<Set *>::iterator end = setsList.end();
    while(it != end)
    {
        if((*it)->isActive())
        {
            activeSet = (*it);
            break;
        }
        it++;
    }
    int amount = camList->count();
    for(int i =0; i < amount; i++)
    {
        if(item == camList->item(i))
        {
            log.writeToFile("Big video from " + camList->getCamera(i)->name());
            activeSet->showBig(i);
            break;
        }
    }
}

void MainWindow::makeSets()
{
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
    for(int i = 0; i < setsList.length(); i++)
        setsList.at(i)->init();
}

void MainWindow::initCameras()
{
    log.writeToFile("Cameras initialization started");
    socket->write("query camera;quantity\n");
    int cameras = readAnswer().trimmed().toInt();
    for(int i = 0; i < cameras; i++)
    {
        QString cam;
        socket->write(QString("query camera;" + QString::number(i+1) + ";name\n").toAscii());
        cam = readAnswer().trimmed();
        log.writeToFile("New camera " + cam);
        camerasList << new Camera(cam);
        socket->write(QString("query camera;" + QString::number(i+1) + ";description\n").toAscii());
        cam = readAnswer().trimmed();
        log.writeToFile("Description " + cam);
        camerasList.at(i)->setDescription(cam);
        socket->write(QString("query camera;" + QString::number(i+1) + ";view:source\n").toAscii());
        cam = readAnswer().trimmed();
        log.writeToFile("Source " + cam);
        camerasList.at(i)->setSource(cam);
        socket->write(QString("query camera;" + QString::number(i+1) + ";view:preview\n").toAscii());
        cam = readAnswer().trimmed();
        log.writeToFile("Preview " + cam);
        camerasList.at(i)->setPreview(cam);
        socket->write(QString("query camera;" + QString::number(i+1) + ";agent\n").toAscii());
        cam = readAnswer().trimmed();
        log.writeToFile("Agent " + cam);
        camerasList.at(i)->setAgent(cam);
    }
    log.writeToFile("Cameras initialization ended");
}

void MainWindow::initSets()
{
    log.writeToFile("Sets initialization started");
    QString info;
    socket->write("query set;quantity\n");
    info = readAnswer().trimmed();
    log.writeToFile("Set amount " + info);
    int sets = info.toInt();
    ui->tabWidget->clear();
    for(int i = 0; i < sets; i++)
    {
        socket->write(QString("query set;" + QString::number(i+1) + ";description\n").toAscii());
        info = readAnswer().trimmed();
        log.writeToFile("New set " + info);
        setsList << new Set(info);
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
    log.writeToFile("Sets initialization ended");
}

void MainWindow::initViews()
{
    log.writeToFile("Views initialization started");
    socket->write("query view;quantity\n");
    QString info = readAnswer().trimmed();
    log.writeToFile("Views amount " + info);
    int views = info.toInt();
    for(int i = 0; i < views; i++)
    {
        socket->write(QString("query view;" + QString::number(i+1) + ";description\n").toAscii());
        info = readAnswer().trimmed();
        log.writeToFile("New view " + info);
        viewsList << new View(info);
        socket->write(QString("query view;" + QString::number(i+1) + ";geometry:width\n").toAscii());
        info = readAnswer().trimmed();
        log.writeToFile("Width: " + info);
        viewsList.at(i)->setWidth(info.toInt());
        socket->write(QString("query view;" + QString::number(i+1) + ";geometry:height\n").toAscii());
        info = readAnswer().trimmed();
        log.writeToFile("Height: " + info);
        viewsList.at(i)->setHeight(info.toInt());
        socket->write(QString("query view;" + QString::number(i+1) + ";geometry:double\n").toAscii());
        info = readAnswer().trimmed();
        log.writeToFile("Double frames: " + info);
        viewsList.at(i)->setDoubleFrames(info.split(','));
        socket->write(QString("query view;" + QString::number(i+1) + ";geometry:triple\n").toAscii());
        info = readAnswer().trimmed();
        log.writeToFile("Triple frames: " + info);
        viewsList.at(i)->setTripleFrames(info.split(','));
        socket->write(QString("query view;" + QString::number(i+1) + ";geometry:quadruple\n").toAscii());
        info = readAnswer().trimmed();
        log.writeToFile("Quadruple frames " + info);
        viewsList.at(i)->setQuadrupleFrames(info.split(','));
        viewsList.at(i)->createIcons();
    }
    log.writeToFile("Views initialization ended");
}

void MainWindow::deleteLogFiles()
{
    log.writeToFile("Action delete other log files clicked");
    QDir d(".");
    d.cd("logs");
    QStringList l;
    l << "*.txt";
    l = d.entryList(l);
    QString currentlog = log.getFileName();
    QStringList::Iterator it = l.begin();
    QStringList::Iterator end = l.end();
    while(it != end)
    {
        QString tmp = d.absoluteFilePath(*it);
        if(tmp != currentlog)
            QFile::remove(tmp);
        it++;
    }
}

void MainWindow::createMenus()
{
    fileMenu = new QMenu(tr("&Файл"),this);
    menuBar()->addMenu(fileMenu);
    helpMenu = new QMenu(tr("Помощь"),this);
    menuBar() -> addMenu(helpMenu);
}

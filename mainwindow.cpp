#include "mainwindow.h"
#include <QList>
#include<QListIterator>
#include<QMessageBox>
#include<QDir>
#include<QMenuBar>
#include"settingsdialog.h"
Logger &log = Logger::instance();
bool test = true;

MainWindow::MainWindow(QWidget *, QString serverAdr, int portNum, bool logging)
{
    settings = new QSettings("Etersoft","VargusViewer");
    bool settingsRead;
    if(serverAdr =="" || portNum == 0)
        settingsRead = readSettings();
    else
    {
        server = serverAdr;
        port = portNum;
        loggingEnabled = logging;
        log.setActive(logging);
        settingsRead = true;
    }
    log.setActive(loggingEnabled);
    if(!log.makeLogFile())
    {
        int n = QMessageBox::warning(this,"Warning",tr("Невозможно открыть файл для записи логов.\n Продолжить работу?"),tr("Да"),tr("Нет"),QString(),0,1);
        if(n) exit(1);
    }
    log.writeToFile("PROGRAM STARTED");
    setTab = new QTabWidget(this);
    connect(setTab, SIGNAL(currentChanged(int)), this, SLOT(onSetChanged(int)));
    createMenus();
    makeButtons();
    createActions();
    camList = new CameraList(this);
    camList->setMaximumWidth(nextButton->width()*4);
    connect(camList,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(makeBigVideo(QListWidgetItem*)));
    setWindowTitle(tr("VargusViewer"));
    createLayouts();
    if(settingsRead)
    {
        startConnection();
    }
    else
        changeConnectionSettings();
}

bool MainWindow::initData()
{
    // Сеанс связи с сервером
    log.writeToFile("Server conection started");
    socket = new QAbstractSocket(QAbstractSocket::TcpSocket, this);
    socket->connectToHost(server, port);
    if(!socket->waitForConnected(5000))
    {
        QMessageBox::critical(NULL, tr("Ошибка"), tr("Невозможно подключиться к серверу.\nПожалуйста, измените настройки соединения"));
        delete socket;
        return false;
    }

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
    return true;
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
    disconnect(setTab,SIGNAL(currentChanged(int)),this,SLOT(onSetChanged(int)));
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
    QList<Set *>::iterator its = setsList.begin();
    QList<Set *>::iterator ends = setsList.end();
    while(its != ends)
    {
        delete (*its);
        its++;
    }
    delete delLogFilesAction;
    delete exitAction;
    delete aboutAction;
    delete enableLog;
    delete fileMenu;
    delete helpMenu;
    delete settingsMenu;
    delete setTab;
    delete centralLayout;
    delete prevButton;
    delete resetButton;
    delete nextButton;

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
            break;
        }
    // Заполнение панели раскладок
    while(!viewLayout->isEmpty())
    {
        QWidget *s = viewLayout->takeAt(0)->widget();
        s->hide();
    }
    for(int i = 0; i < viewsList.count(); i++)
    {
        View *view = setsList.at(setTab->currentIndex())->views().at(i);
        view->show();
        viewLayout->addWidget(view, i / 2, i % 2);
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
    exitAction = new QAction(tr("&Выход"),this);
    fileMenu->addAction(exitAction);
    connect(exitAction,SIGNAL(triggered()),this,SLOT(close()));

    aboutAction = new QAction(tr("&О программе"),this);
    helpMenu->addAction(aboutAction);
    connect(aboutAction,SIGNAL(triggered()),this,SLOT(about()));

    enableLog = new QAction(tr("&Вести лог"),this);
    enableLog -> setCheckable(true);
    enableLog -> setChecked(loggingEnabled);
    settingsMenu -> addAction(enableLog);
    connect(enableLog,SIGNAL(toggled(bool)),this,SLOT(enableLogging(bool)));

    connectionSettings = new QAction(tr("&Настройки соединения"),this);
    settingsMenu -> addAction(connectionSettings);
    connect(connectionSettings,SIGNAL(triggered()),this,SLOT(changeConnectionSettings()));

}

void MainWindow::about()
{
    log.writeToFile("Action about clicked");
    QMessageBox::about(this, tr("О программе"),
             tr("<h2>VargusViewer</h2><p>Etersoft 2012</p>"));

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
    QList<Set *>::iterator it =setsList.begin();
    QList<Set *>::iterator end = setsList.end();
    while(it != end)
    {
        if((*it) -> isActive())
            camList->setCurrentCameras((*it) -> cameras());
        it++;
    }
    camList -> setActiveCameras(activeCameras);
    camList -> print();
}

void MainWindow::makeButtons()
{
    log.writeToFile("Making buttons");
    prevButton = new QPushButton(this);
    prevButton->setMinimumSize(50,50);
    prevButton->setMaximumSize(50,50);
    resetButton = new QPushButton(this);
    resetButton->setMinimumSize(50,50);
    resetButton->setMaximumSize(50,50);
    nextButton = new QPushButton(this);
    nextButton->setMinimumSize(50,50);
    nextButton->setMaximumSize(50,50);
    prevButton->setIcon(QIcon("images/prev.png"));
    resetButton->setIcon(QIcon("images/reset.png"));
    nextButton->setIcon(QIcon("images/next.png"));
    connect(nextButton,SIGNAL(clicked()),this,SLOT(nextGroup()));
    connect(prevButton,SIGNAL(clicked()),this,SLOT(prevGroup()));
    connect(resetButton,SIGNAL(clicked()),this,SLOT(resetGroup()));
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
    Set *activeSet = NULL;
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
        set->init();
        set->setActiveView(0);
        setTab->addTab(set, set->description());
        connect(set,SIGNAL(updateActiveCameras(QList<Camera*>)),this,SLOT(changeActiveCameras(QList<Camera*>)));
        connect(set,SIGNAL(windowIsVisible(bool)),this,SLOT(setVisible(bool)));

    }
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
    menuBar() -> addMenu(fileMenu);
    settingsMenu = new QMenu(tr("&Настройки"),this);
    menuBar() -> addMenu(settingsMenu);
    helpMenu = new QMenu(tr("&Помощь"),this);
    menuBar() -> addMenu(helpMenu);
}

void MainWindow::createLayouts()
{
    QWidget *w = new QWidget(this);

    centralLayout = new QHBoxLayout();

    videoLayout = new QVBoxLayout();
    videoLayout -> addWidget(setTab);
    centralLayout -> addLayout(videoLayout);

    controlLayout = new QVBoxLayout();

    viewLayout = new QGridLayout();
    controlLayout -> addLayout(viewLayout);

    buttonLayout = new QHBoxLayout();
    buttonLayout -> setMargin(0);
    buttonLayout -> addWidget(prevButton);
    buttonLayout -> addWidget(resetButton);
    buttonLayout -> addWidget(nextButton);
    controlLayout -> addLayout(buttonLayout);
    controlLayout -> addWidget(camList);
    centralLayout -> addLayout(controlLayout);
    w->setLayout(centralLayout);
    setCentralWidget(w);
    setMinimumSize(800,600);
}

void MainWindow::enableLogging(bool enable)
{
    if(enable)
    {
        log.setActive(enable);
        settings->setValue("logging",1);
        log.writeToFile("Logging is enabled");
    }
    else {
        settings->setValue("logging",0);
        log.writeToFile("Logging is disbled");
        log.setActive(enable);
    }
    loggingEnabled = enable;
}

bool MainWindow::readSettings()
{
    server = settings->value("server","").toString();
    port = settings->value("port",-1).toInt();
    int t = settings->value("logging", -1).toInt();
    if(t == 1 || t ==-1)
        loggingEnabled = true;
    else loggingEnabled = false;
    if(server == "" || port < 0 || port > 65535)
        return false;
    return true;
}

void MainWindow::changeConnectionSettings()
{
    SettingsDialog d(this,server,port);
    connect(&d,SIGNAL(newSettings(QString,int)),this,SLOT(newSettings(QString,int)));
    d.exec();
}

void MainWindow::newSettings(QString newServer, int newPort)
{
    if((server == newServer) && (port == newPort))
        return;
    server = newServer;
    port = newPort;
    saveSettings();
    startConnection();
}

void MainWindow::saveSettings()
{
    if(!settings) return;
    settings->setValue("server",server);
    settings->setValue("port",port);
    if(loggingEnabled)
        settings->setValue("logging",1);
    else
        settings->setValue("logging",0);
}

void MainWindow::startConnection()
{
    disconnect(setTab,SIGNAL(currentChanged(int)),this,SLOT(onSetChanged(int)));

    QList<Camera *>::iterator itc = camerasList.begin();
    QList<Camera *>::iterator endc = camerasList.end();
    while(itc != endc)
    {
        delete (*itc);
        itc++;
    }
    camList->clear();
    camerasList.clear();
    QList<View *>::iterator itv = viewsList.begin();
    QList<View *>::iterator endv = viewsList.end();
    while(itv != endv)
    {
        delete (*itv);
        itv++;
    }
    viewsList.clear();
    setTab->clear();
    QList<Set *>::iterator its = setsList.begin();
    QList<Set *>::iterator ends = setsList.end();
    while (its != ends)
    {
        delete (*its);
        its++;
    }
    setsList.clear();
    // Обработка входных данных
    if(initData() == false)
        return;
    // Заполнение вкладок-сетов
    makeSets();
    setTab->setCurrentIndex(0);
    onSetChanged(0);
    connect(setTab,SIGNAL(currentChanged(int)),this,SLOT(onSetChanged(int)));
    changeActiveCameras(setsList.at(0)->getActiveCameras());
}

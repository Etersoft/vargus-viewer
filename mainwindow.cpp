#include "mainwindow.h"
#include <QList>
#include<QListIterator>
#include<QMessageBox>
#include<QDir>
#include<QMenuBar>
#include"settingsdialog.h"
#include <QtCore>
Logger &vargusLog = Logger::instance();
bool test = true;

MainWindow::MainWindow(QWidget *, QString serverAdr, int portNum, bool logging)
{
    createIcons();
    settings = new QSettings("Etersoft","VargusViewer");
    bool settingsRead;
    if(serverAdr =="" || portNum == 0)
        settingsRead = readSettings();
    else
    {
        server = serverAdr;
        port = portNum;
        loggingEnabled = logging;
        vargusLog.setActive(logging);
        settingsRead = true;
    }
    vargusLog.setActive(loggingEnabled);
    if(!vargusLog.makeLogFile())
    {
        int n = QMessageBox::warning(this,"Warning",tr("Невозможно открыть файл для записи логов.\n Продолжить работу?"),tr("Да"),tr("Нет"),QString(),0,1);
        if(n) exit(1);
    }
    vargusLog.writeToFile("PROGRAM STARTED");
    setTab = new QTabWidget(this);
    connect(setTab, SIGNAL(currentChanged(int)), this, SLOT(onSetChanged(int)));
    createMenus();
    makeButtons();
    createActions();
    camList = new CameraList(this);
    camList -> setMaximumWidth(nextButton -> width() * 4);
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
    vargusLog.writeToFile("Server conection started");
    socket = new QAbstractSocket(QAbstractSocket::TcpSocket, this);
    socket -> connectToHost(server, port);
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

    socket -> write("exit\n");
    socket -> disconnect();
    delete socket;
    vargusLog.writeToFile("Connection closed. Initialization ended");
    return true;
}


QStringList MainWindow::readAnswer(int amountOfLines)
{
    QStringList ans;
    int tries = 0;
    while(tries < 5)
    {
        if(amountOfLines < 1)
            break;
        QString answer;
        if(!socket->waitForReadyRead(2000))
        {
            tries++;
            continue;
        }
        answer = trUtf8(socket->readLine().data());
        if( answer.at(0) != '>' && answer.at(0) != ':' )
        {
            ans.push_back(answer);
            amountOfLines--;
        }
    }
    if (tries == 5)
        QMessageBox::critical(this, tr("Error"), tr("Server is not response"));
    return ans;
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
    delete fpsCounterAction;
    delete enableLog;
    delete fileMenu;
    delete helpMenu;
    delete settingsMenu;
    delete setTab;
    delete centralLayout;
    delete prevButton;
    delete resetButton;
    delete nextButton;
    delete trIcon;

    vargusLog.writeToFile("PROGRAM ENDED");
    vargusLog.closeFile();
}

void MainWindow::onSetChanged(int num)
{
    vargusLog.writeToFile("New active set " + setsList.at(num) -> description());
    for( int i = 0; i < setsList.length(); i++)
        if(setsList.at(i) -> isActive())
        {
            setsList.at(i) -> stopPlay();
            setsList.at(i) -> setActive(false);
            break;
        }
    // Заполнение панели раскладок
    while(!viewLayout -> isEmpty())
    {
        QWidget *s = viewLayout -> takeAt(0) -> widget();
        s -> hide();
    }
    for(int i = 0; i < viewsList.count(); i++)
    {
        View *view = setsList.at(setTab->currentIndex())->views().at(i);
        view -> show();
        viewLayout -> addWidget(view, i / 2, i % 2);
    }
    setsList.at(num) -> setActive(true);
    setsList.at(num) -> restoreState();
}

void MainWindow::createActions()
{
    delLogFilesAction = new QAction(tr("&Удалить log файлы"),this);
    fileMenu->addAction(delLogFilesAction);
    connect(delLogFilesAction,SIGNAL(triggered()),this,SLOT(deleteLogFiles()));

    fpsCounterAction = new QAction(tr("&Кадры в секунду"),this);
    fileMenu -> addAction(fpsCounterAction);
    connect(fpsCounterAction, SIGNAL(triggered()), this, SLOT(showFPS()));

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
    vargusLog.writeToFile("Action about clicked");
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
            camList -> setCurrentCameras((*it) -> cameras());
        it++;
    }
    camList -> setActiveCameras(activeCameras);
    camList -> print();
}

void MainWindow::makeButtons()
{
    QString imagePath = "/usr/share/vargus-viewer/images/";
    vargusLog.writeToFile("Making buttons");
    prevButton = new QPushButton(this);
    prevButton -> setMinimumSize(50,50);
    prevButton -> setMaximumSize(50,50);
    resetButton = new QPushButton(this);
    resetButton -> setMinimumSize(50,50);
    resetButton -> setMaximumSize(50,50);
    nextButton = new QPushButton(this);
    nextButton -> setMinimumSize(50,50);
    nextButton -> setMaximumSize(50,50);
    prevButton -> setIcon(QIcon(imagePath + "prev.png"));
    resetButton -> setIcon(QIcon(imagePath + "reset.png"));
    nextButton -> setIcon(QIcon(imagePath + "next.png"));
    QSize s(32,32);
    prevButton -> setIconSize(s);
    resetButton -> setIconSize(s);
    nextButton -> setIconSize(s);
    connect(nextButton,SIGNAL(clicked()),this,SLOT(nextGroup()));
    connect(prevButton,SIGNAL(clicked()),this,SLOT(prevGroup()));
    connect(resetButton,SIGNAL(clicked()),this,SLOT(resetGroup()));
    vargusLog.writeToFile("Buttons made");
}

void MainWindow::nextGroup()
{
    vargusLog.writeToFile("Clicked next button");
    QList<Set *>::iterator it = setsList.begin();
    QList<Set *>::iterator end = setsList.end();
    while(it != end)
    {
        if((*it) -> isActive())
        {
            (*it) -> next();
            break;
        }
        it++;
    }
}

void MainWindow::prevGroup()
{
    vargusLog.writeToFile("Clicked previous button");
    QList<Set *>::iterator it = setsList.begin();
    QList<Set *>::iterator end = setsList.end();
    while(it != end)
    {
        if((*it) -> isActive())
        {
            (*it) -> prev();
            break;
        }
        it++;
    }
}

void MainWindow::resetGroup()
{
    vargusLog.writeToFile("Clicked reset button");
    QList<Set *>::iterator it = setsList.begin();
    QList<Set *>::iterator end = setsList.end();
    while(it != end)
    {
        if((*it) -> isActive())
        {
            (*it) -> reset();
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
        if((*it) -> isActive())
        {
            activeSet = (*it);
            break;
        }
        it++;
    }
    int amount = camList -> count();
    for(int i = 0; i < amount; i++)
    {
        if(item == camList -> item(i))
        {
            vargusLog.writeToFile("Big video from " + camList -> getCamera(i) -> name());
            activeSet -> showBig(i);
            break;
        }
    }
}

void MainWindow::makeSets()
{
    QList<Set *>::iterator it = setsList.begin();
    QList<Set *>::iterator end = setsList.end();
    while( it != end )
    {
        Set *set = (*it);
        QList<View *>::iterator itv = viewsList.begin();
        QList<View *>::iterator endv = viewsList.end();
        while( itv != endv )
        {
            set -> addView(*itv);
            itv++;
        }
        set -> init();
        set -> setActiveView(0);
        setTab -> addTab(set, set->description());
        connect(set, SIGNAL(updateActiveCameras(QList<Camera*>)), this, SLOT(changeActiveCameras(QList<Camera*>)));
        connect(set, SIGNAL(buttonsEnabled(bool, bool)), this, SLOT(enableButtons(bool, bool)));
        it++;
    }
}

void MainWindow::initCameras()
{
    vargusLog.writeToFile("Cameras initialization started");
    socket -> write("query camera;quantity\n");
    int cameras = readAnswer().at(0).trimmed().toInt();
    vargusLog.writeToFile("Amount of cameras " + QString::number(cameras));
    QString camerasnumbers;
    for(int i = 1; i < cameras; i++)
        camerasnumbers += (QString::number(i) + ',');
    camerasnumbers += QString::number(cameras);
    socket -> write(QString("query camera;" + camerasnumbers +
                          ";name,description,view:source,view:preview,agent\n").toAscii());
    QStringList inf = readAnswer(cameras);
    for(int i = 0; i < cameras; i++)
    {
        QStringList cam = inf.at(i).split(';');
        vargusLog.writeToFile("New camera " + cam.at(0));
        Camera *c = new Camera(cam.at(0));
        vargusLog.writeToFile("Description " + cam.at(1));
        c -> setDescription(cam.at(1));
        vargusLog.writeToFile("Source " + cam.at(2));
        c -> setSource(cam.at(2));
        vargusLog.writeToFile("Preview " + cam.at(3));
        c -> setPreview(cam.at(3));
        vargusLog.writeToFile("Agent " + cam.at(4).trimmed());
        c -> setAgent(cam.at(4).trimmed());
        camerasList << c;
    }
    vargusLog.writeToFile("Cameras initialization ended");
}

void MainWindow::initSets()
{
    vargusLog.writeToFile("Sets initialization started");
    socket -> write("query set;quantity\n");
    int sets = readAnswer().at(0).trimmed().toInt();
    vargusLog.writeToFile("Set amount " + QString::number(sets));
    QString setsnumbers;
    for(int i = 1; i < sets; i++)
        setsnumbers += (QString::number(i) + ',');
    setsnumbers += QString::number(sets);
    socket -> write(QString("query set;" + setsnumbers + ";description,cameras\n").toAscii());
    QStringList inf = readAnswer(sets);
    for(int i = 0; i < sets; i++)
    {
        QStringList setinfo = inf.at(i).split(';');
        QString info = setinfo.at(0);
        vargusLog.writeToFile("New set " + info);
        Set * s = new Set(info);
        setsList << s;
        QStringList camlist = setinfo.at(1).trimmed().split(',');
        for(int j = 0; j < camlist.count(); j++)
        {
            QList<Camera *>::iterator it = camerasList.begin();
            QList<Camera *>::iterator end = camerasList.end();
            while( it != end )
            {
                if(camlist.at(j) == (*it)->name())
                {
                    s -> addCamera(*it);
                    break;
                }
                it++;
            }
        }
    }
    vargusLog.writeToFile("Sets initialization ended");
}

void MainWindow::initViews()
{
    vargusLog.writeToFile("Views initialization started");
    socket -> write("query view;quantity\n");
    int views = readAnswer().at(0).trimmed().toInt();
    vargusLog.writeToFile("Views amount " + QString::number(views));
    QString viewsnumbers;
    for(int i = 1; i < views; i++)
        viewsnumbers += (QString::number(i) + ',');
    viewsnumbers += QString::number(views);
    socket -> write(QString("query view;" + viewsnumbers +
      ";description,geometry:width,geometry:height,geometry:double,geometry:triple,geometry:quadruple\n").toAscii());
    QStringList inf = readAnswer(views);
    for(int i = 0; i < views; i++)
    {
        QStringList info = inf.at(i).split(';');
        vargusLog.writeToFile("New view " + info.at(0));
        View *v = new View(info.at(0));
        vargusLog.writeToFile("Width: " + info.at(1));
        v -> setWidth(info.at(1).toInt());
        vargusLog.writeToFile("Height: " + info.at(2));
        v -> setHeight(info.at(2).toInt());
        vargusLog.writeToFile("Double frames: " + info.at(3));
        v -> setDoubleFrames(info.at(3).trimmed().split(','));
        vargusLog.writeToFile("Triple frames: " + info.at(4));
        v -> setTripleFrames(info.at(4).trimmed().split(','));
        vargusLog.writeToFile("Quadruple frames " + info.at(5));
        v -> setQuadrupleFrames(info.at(5).trimmed().split(','));
        v -> createIcons();
        viewsList << v;
    }
    vargusLog.writeToFile("Views initialization ended");
}

void MainWindow::deleteLogFiles()
{
    vargusLog.writeToFile("Action delete other log files clicked");
    QDir d(".");
    d.cd("logs");
    QStringList l;
    l << "*.txt";
    l = d.entryList(l);
    QString currentlog = vargusLog.getFileName();
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
    w -> setLayout(centralLayout);
    setCentralWidget(w);
    setMinimumSize(800,600);
}

void MainWindow::enableLogging(bool enable)
{
    if(enable)
    {
        vargusLog.setActive(enable);
        settings -> setValue("logging",1);
        vargusLog.writeToFile("Logging is enabled");
    }
    else {
        settings -> setValue("logging",0);
        vargusLog.writeToFile("Logging is disbled");
        vargusLog.setActive(enable);
    }
    loggingEnabled = enable;
}

bool MainWindow::readSettings()
{
    server = settings -> value("server", "").toString();
    port = settings -> value("port", -1).toInt();
    int t = settings -> value("logging", -1).toInt();
    if(t == 1 || t == -1)
        loggingEnabled = true;
    else loggingEnabled = false;
    if(server == "" || port < 0 || port > 65535)
        return false;
    return true;
}

void MainWindow::changeConnectionSettings()
{
    SettingsDialog d(this,server,port);
    connect(&d, SIGNAL(newSettings(QString, int)), this, SLOT(newSettings(QString, int)));
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
    settings -> setValue("server",server);
    settings -> setValue("port",port);
    if(loggingEnabled)
        settings -> setValue("logging",1);
    else
        settings -> setValue("logging",0);
}

void MainWindow::startConnection()
{
    disconnect(setTab, SIGNAL(currentChanged(int)), this, SLOT(onSetChanged(int)));

    QList<Camera *>::iterator itc = camerasList.begin();
    QList<Camera *>::iterator endc = camerasList.end();
    while(itc != endc)
    {
        delete (*itc);
        itc++;
    }
    camList -> clear();
    camerasList.clear();
    QList<View *>::iterator itv = viewsList.begin();
    QList<View *>::iterator endv = viewsList.end();
    while(itv != endv)
    {
        delete (*itv);
        itv++;
    }
    viewsList.clear();
    setTab -> clear();
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
    setTab -> setCurrentIndex(0);
    onSetChanged(0);
    connect(setTab, SIGNAL(currentChanged(int)), this, SLOT(onSetChanged(int)));
    changeActiveCameras(setsList.at(0) -> getActiveCameras());
}

void MainWindow::showFPS()
{
    QList<Set *>::iterator it = setsList.begin();
    QList<Set *>::iterator end = setsList.end();
    while( it!= end )
    {
        if( (*it) -> isActive() )
        {
           countFPS((*it) -> video());
           break;
        }
        it++;
    }
}

void MainWindow::countFPS(const QList<VideoWidget *> &video)
{
    FPSCounter counter(this);
    counter.setVideoList(video);
    counter.start();
    counter.exec();
}

void MainWindow::enableButtons(bool prev, bool next)
{
    prevButton -> setEnabled(prev);
    nextButton -> setEnabled(next);
}

void MainWindow::createIcons()
{
    QString imagePath = "/usr/share/vargus-viewer/images/";
    trIcon = new QSystemTrayIcon();  //инициализируем объект
    trIcon->setIcon(QIcon(imagePath + "vargus32.png"));  //устанавливаем иконку
    trIcon->show();  //отображаем объект
    connect(trIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(showHide(QSystemTrayIcon::ActivationReason)));
}


void MainWindow::showHide(QSystemTrayIcon::ActivationReason r) {
    if (r == QSystemTrayIcon::Trigger)  //если нажато левой кнопкой продолжаем
    {
        if (!isVisible()) {  //если окно было не видимо - отображаем его
            show();
        } else {
            hide();  //иначе скрываем
        }
    }
}

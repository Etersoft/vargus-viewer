#include "mainwindow.h"
#include <QList>
#include <QListIterator>
#include <QMessageBox>
#include <QDir>
#include <QMenuBar>
#include "settingsdialog.h"
#include <QtCore>
#include <QFileDialog>
#include <logger.h>
#include <videowidget.h>
#include "logsettings.h"
#include "fontsettings.h"
#include "painttextproperties.h"

extern Logger &vargusLog;

MainWindow::MainWindow(QWidget *, QString serverAdr, int portNum, bool logging,int setNum, int viewNum)
{
    createIcons();
    settings = new QSettings("Etersoft","VargusViewer");
    bool settingsRead;
    //read settings for first initialisation of parametrs
    readSettings();
    if(!serverAdr.isEmpty())
        server = serverAdr;
    if(portNum != 0)
        port = portNum;
    if(logging == false)
        loggingEnabled = false;
    if(setNum != -1)
        set = setNum;
    else set = 0;
    if(viewNum != -1)
        view = viewNum;
    else view = 0;
    //save settings, because commandline options may be here
    saveSettings();
    settingsRead = readSettings();

    Camera::setRunningTextAddress(t_server, t_port);
    vargusLog.changeDirectory(pathForLogs);
    vargusLog.setActive(loggingEnabled);
    if(!vargusLog.makeLogFile())
    {
        int n = QMessageBox::warning(
                    this, tr("Warning"),
                    tr("Can not open file for logging. \n Continue to work?"),
                    tr("Yes"), tr("No"), QString(), 0, 1);
        if(n) exit(1);
    }
    vargusLog.writeToFile("PROGRAM STARTED");
    //make main tab widget
    setTab = new QTabWidget(this);
    connect(setTab, SIGNAL(currentChanged(int)), this, SLOT(onSetChanged(int)));
    createMenus();
    makeButtons();
    createActions();
    //make cameralist. If cam is double clicked, video must be fullsize
    camList = new CameraList(this);
    camList->setMaximumWidth(nextButton->width() * 4);
    connect(camList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(makeBigVideo(QListWidgetItem*)));
    setWindowTitle(tr("Vargus Viewer"));
    createLayouts();
    //connect ro server if ettings were read good
    if(settingsRead)
    {
        startConnection();
    }
    else
        //settings are bad
        changeConnectionSettings();
}

bool MainWindow::initData()
{
    // Connect to server
    vargusLog.writeToFile("Server conection started");
    socket = new QAbstractSocket(QAbstractSocket::TcpSocket, this);
    socket->connectToHost(server, port);
    if(!socket->waitForConnected(5000))
    {
        QMessageBox::critical(this,
                              tr("Error"), tr("Can not connect to server.\nPlease, change the connection settings."));
        delete socket;
        return false;
    }

    // initialisations
    initCameras();
    initSets();
    initViews();

    socket->write("exit\n");
    socket->disconnect();
    delete socket;
    vargusLog.writeToFile("Connection closed. Initialization ended");
    return true;
}

//read answer from sever
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
        if(!answer.isEmpty() && answer.at(0) != '>' && answer.at(0) != ':' )
        {
            ans.push_back(answer);
            amountOfLines--;
        }
    }
    if (tries == 5)
    {
        QMessageBox b(QMessageBox::Critical, tr("Error"), tr("Server is not response."));
        b.exec();
        exit(2);
    }
    return ans;
}

MainWindow::~MainWindow()
{
    disconnect(setTab, SIGNAL(currentChanged(int)), this, SLOT(onSetChanged(int)));
    foreach(View *v, viewsList)
        delete v;
    vargusLog.writeToFile("Destroy of views success");
    foreach(Set *s, setsList)
        s->stopPlay();
    vargusLog.writeToFile("Stop play of each set success");
    VideoWidget::staticDestructor();
    vargusLog.writeToFile("staticDestructor of VideoWidget success");

    foreach(Camera *c, camerasList)
        delete c;
    vargusLog.writeToFile("Destroy of cameras success");
    foreach(Set *s, setsList)
        delete s;
    vargusLog.writeToFile("Destroy of sets success");

    delete exitAction;
    delete aboutAction;
    delete fileMenu;
    delete helpMenu;
    delete settingsMenu;
    delete contextMenu;
    delete setTab;
    delete centralLayout;
    delete prevButton;
    delete resetButton;
    delete nextButton;
    delete trIcon;
    delete logAction;
    delete fontsettingsAction;
    delete connectionSettings;
    delete vlcsettingsAction;
    vargusLog.writeToFile("PROGRAM ENDED");
}

//if new tab selected
void MainWindow::onSetChanged(int num)
{
    vargusLog.writeToFile("New active set " + setsList.at(num)->description());
    foreach(Set *s, setsList)
    {
        if(s->isActive())
        {
            s->stopPlay();
            s->setActive(false);
            break;
        }
    }

    // show new view's from selected set
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
    exitAction = new QAction(tr("&Exit"), this);
    fileMenu->addAction(exitAction);
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    aboutAction = new QAction(tr("&About"), this);
    helpMenu->addAction(aboutAction);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

    connectionSettings = new QAction(tr("&Connection settings"), this);
    settingsMenu->addAction(connectionSettings);
    connect(connectionSettings, SIGNAL(triggered()), this, SLOT(changeConnectionSettings()));

    vlcsettingsAction = new QAction(tr("&VLC settings"), this);
    settingsMenu->addAction(vlcsettingsAction);
    connect(vlcsettingsAction, SIGNAL(triggered()), this, SLOT(vlcsettingsDialog()));

    contextMenu->addAction(connectionSettings);
    contextMenu->addAction(exitAction);

    logAction = new QAction(tr("&Log settings"), this);
    settingsMenu->addAction(logAction);
    connect(logAction, SIGNAL(triggered()), this, SLOT(logSettings()));

    fontsettingsAction = new QAction(tr("&Font settings"), this);
    settingsMenu->addAction(fontsettingsAction);
    connect(fontsettingsAction, SIGNAL(triggered()), this, SLOT(fontSettings()));
}

void MainWindow::about()
{
    vargusLog.writeToFile("Action about clicked");
    QMessageBox::about(this, tr("About"),
             tr("<h2>Vargus Viewer</h2><p>Etersoft 2012</p>"));
}

//will asks if user really want to exit
void MainWindow::closeEvent(QCloseEvent *event)
{
    if(okToContinue())
        QMainWindow::closeEvent(event);
    else event->ignore();
}

//ask user about exit
bool MainWindow::okToContinue()
{
    int r = QMessageBox::warning(this, tr("Exit program"), tr("Are you sure you want to quit?"),
                                 QMessageBox::Yes | QMessageBox::No);
    if(r == QMessageBox::Yes)
        return true;
    else return false;
}

//if there are new cameras in active set's layout, need to update camera list
void MainWindow::changeActiveCameras(QList<Camera *> activeCameras)
{
    foreach(Set *s, setsList)
    {
        if(s->isActive())
            camList->setCurrentCameras(s->cameras());
    }
    camList->setActiveCameras(activeCameras);
    camList->print();
}

void MainWindow::makeButtons()
{
    QString imagePath = QString(DATADIR) + "images/";
    vargusLog.writeToFile("Making buttons");
    prevButton = new QPushButton(this);
    prevButton->setFixedSize(50, 50);
    prevButton->setToolTip(tr("Previous cameras"));
    resetButton = new QPushButton(this);
    resetButton->setFixedSize(50, 50);
    resetButton->setToolTip(tr("Reset"));
    nextButton = new QPushButton(this);
    nextButton->setFixedSize(50, 50);
    nextButton->setToolTip(tr("Next cameras"));
    prevButton->setIcon(QIcon(imagePath + "prev.png"));
    resetButton->setIcon(QIcon(imagePath + "reset.png"));
    nextButton->setIcon(QIcon(imagePath + "next.png"));
    QSize s(32, 32);
    prevButton->setIconSize(s);
    resetButton->setIconSize(s);
    nextButton->setIconSize(s);
    connect(nextButton, SIGNAL(clicked()), this, SLOT(nextGroup()));
    connect(prevButton, SIGNAL(clicked()), this, SLOT(prevGroup()));
    connect(resetButton, SIGNAL(clicked()), this, SLOT(resetGroup()));
    vargusLog.writeToFile("Buttons made");
}

//clicked next button
void MainWindow::nextGroup()
{
    vargusLog.writeToFile("Clicked next button");
    foreach(Set *s, setsList)
    {
        if(s->isActive())
        {
            s->next();
            break;
        }
    }

}

//previous button clicked
void MainWindow::prevGroup()
{
    vargusLog.writeToFile("Clicked previous button");
    foreach(Set *s, setsList)
    {
        if(s->isActive())
        {
            s->prev();
            break;
        }
    }
}

//reset button clicked
void MainWindow::resetGroup()
{
    vargusLog.writeToFile("Clicked reset button");
    foreach(Set *s, setsList)
    {
        if(s->isActive())
        {
            s->reset();
            break;
        }
    }
}

//if camera at camera list was double clicked, we need to show fullsize video
void MainWindow::makeBigVideo(QListWidgetItem * item)
{
    Set *activeSet = NULL;
    foreach(activeSet, setsList)//find active set
    {
        if(activeSet->isActive())
            break;
    }
    int amount = camList->count();
    for(int i = 0; i < amount; i++)
    {
        if(item == camList->item(i))
        {
            vargusLog.writeToFile("Big video from " + camList->getCamera(i)->name());
            activeSet->showBig(i);
            break;
        }
    }
}

void MainWindow::makeSets()
{
    int i = 0;
    foreach(Set *set, setsList)
    {
        foreach(View *v, viewsList)
            set->addView(v);
        set->init();
        if((i == this->set) && (view < viewsList.size()))
            set->setActiveView(view);
        else
            set->setActiveView(0);
        setTab->addTab(set, set->description());
        connect(set, SIGNAL(updateActiveCameras(QList<Camera*>)), this, SLOT(changeActiveCameras(QList<Camera*>)));
        connect(set, SIGNAL(buttonsEnabled(bool, bool)), this, SLOT(enableButtons(bool, bool)));
        i++;
    }
}

//sending requests to the server about cameras
void MainWindow::initCameras()
{
    vargusLog.writeToFile("Cameras initialization started");
    socket->write("query camera;quantity\n");
    int cameras = readAnswer().at(0).trimmed().toInt();
    vargusLog.writeToFile("Amount of cameras " + QString::number(cameras));
    QString camerasnumbers;
    for(int i = 1; i < cameras; i++)
        camerasnumbers += (QString::number(i) + ',');
    camerasnumbers += QString::number(cameras);
    socket->write(QString("query camera;" + camerasnumbers +
                          ";name,description,view:source,view:preview,agent\n").toAscii());
    QStringList inf = readAnswer(cameras);
    for(int i = 0; i < cameras; i++)
    {
        QStringList cam = inf.at(i).split(';');
        vargusLog.writeToFile("New camera " + cam.at(0));
        Camera *c = new Camera(cam.at(0));
        vargusLog.writeToFile("Description " + cam.at(1));
        c->setDescription(cam.at(1));
        vargusLog.writeToFile("Source " + cam.at(2));
        c->setSource(cam.at(2));
        vargusLog.writeToFile("Preview " + cam.at(3));
        c->setPreview(cam.at(3));
        vargusLog.writeToFile("Agent " + cam.at(4).trimmed());
        c->setAgent(cam.at(4).trimmed());
        c->setNumber(i+1);
        camerasList << c;
    }
    vargusLog.writeToFile("Cameras initialization ended");
}

//sending requests to the server about sets
void MainWindow::initSets()
{
    vargusLog.writeToFile("Sets initialization started");
    socket->write("query set;quantity\n");
    int sets = readAnswer().at(0).trimmed().toInt();
    vargusLog.writeToFile("Set amount " + QString::number(sets));
    QString setsnumbers;
    for(int i = 1; i < sets; i++)
        setsnumbers += (QString::number(i) + ',');
    setsnumbers += QString::number(sets);
    socket->write(QString("query set;" + setsnumbers + ";description,cameras\n").toAscii());
    QStringList inf = readAnswer(sets);
    for(int i = 0; i < sets; i++)
    {
        QStringList setinfo = inf.at(i).split(';');
        QString info = setinfo.at(0);
        vargusLog.writeToFile("New set " + info);
        Set * s = new Set(info, this, server);
        setsList << s;
        QStringList camlist = setinfo.at(1).trimmed().split(',');
        for(int j = 0; j < camlist.count(); j++)
        {
            foreach(Camera *c, camerasList)
            {
                if(camlist.at(j) == c->name())
                {
                    s->addCamera(c);
                    break;
                }
            }
        }
    }
    vargusLog.writeToFile("Sets initialization ended");
}

//sending requests to the server about views
void MainWindow::initViews()
{
    vargusLog.writeToFile("Views initialization started");
    socket->write("query view;quantity\n");
    int views = readAnswer().at(0).trimmed().toInt();
    vargusLog.writeToFile("Views amount " + QString::number(views));
    QString viewsnumbers;
    for(int i = 1; i < views; i++)
        viewsnumbers += (QString::number(i) + ',');
    viewsnumbers += QString::number(views);
    socket->write(QString("query view;" + viewsnumbers +
      ";description,geometry:width,geometry:height,geometry:double,geometry:triple,geometry:quadruple\n").toAscii());
    QStringList inf = readAnswer(views);
    for(int i = 0; i < views; i++)
    {
        QStringList info = inf.at(i).split(';');
        vargusLog.writeToFile("New view " + info.at(0));
        View *v = new View(info.at(0));
        vargusLog.writeToFile("Width: " + info.at(1));
        v->setWidth(info.at(1).toInt());
        vargusLog.writeToFile("Height: " + info.at(2));
        v->setHeight(info.at(2).toInt());
        vargusLog.writeToFile("Double frames: " + info.at(3));
        v->setDoubleFrames(info.at(3).trimmed().split(','));
        vargusLog.writeToFile("Triple frames: " + info.at(4));
        v->setTripleFrames(info.at(4).trimmed().split(','));
        vargusLog.writeToFile("Quadruple frames " + info.at(5));
        v->setQuadrupleFrames(info.at(5).trimmed().split(','));
        v->createIcons();
        viewsList << v;
    }
    vargusLog.writeToFile("Views initialization ended");
}

void MainWindow::createMenus()
{

    fileMenu = new QMenu(tr("&File"), this);
    menuBar()->addMenu(fileMenu);
    settingsMenu = new QMenu(tr("&Settings"), this);
    menuBar()->addMenu(settingsMenu);
    helpMenu = new QMenu(tr("&Help"), this);
    menuBar()->addMenu(helpMenu);
    contextMenu = new QMenu(this);
    menuBar()->addMenu(contextMenu);
    trIcon->setContextMenu(contextMenu);
}

void MainWindow::createLayouts()
{
    QWidget *w = new QWidget(this);

    centralLayout = new QHBoxLayout();

    videoLayout = new QVBoxLayout();
    videoLayout->addWidget(setTab);
    centralLayout->addLayout(videoLayout);

    controlLayout = new QVBoxLayout();

    viewLayout = new QGridLayout();
    controlLayout->addLayout(viewLayout);

    buttonLayout = new QHBoxLayout();
    buttonLayout->setMargin(0);
    buttonLayout->addWidget(prevButton);
    buttonLayout->addWidget(resetButton);
    buttonLayout->addWidget(nextButton);
    controlLayout->addLayout(buttonLayout);
    controlLayout->addWidget(camList);
    centralLayout->addLayout(controlLayout);
    w->setLayout(centralLayout);
    setCentralWidget(w);
    setMinimumSize(800,600);
}

bool MainWindow::readSettings()
{
    server = settings->value("server", "").toString();
    port = settings->value("port", 0).toInt();
    int t = settings->value("logging", -1).toInt();
    t_server = settings->value("t_server", "").toString();
    t_port = settings->value("t_port", 0).toInt();

    if(t == 1 || t == -1)
        loggingEnabled = true;
    else loggingEnabled = false;
    pathForLogs = settings->value("Directory for logs","").toString();
    if(pathForLogs == "")
    {
        pathForLogs = WORKDIR;
        pathForLogs += "logs/";
    }
    vlcSettings = settings->value("vlcsettings", "").toString();
    //vlc settings defaults if there are no settings in memory
    if(vlcSettings == "")
    {
        vlcSettings.clear();
        vlcSettings.append("-I dummy"); /* Don't use any interface */
        vlcSettings.append(" --ignore-config"); /* Don't use VLC's config */
        vlcSettings.append(" --no-audio"); /* Audio off */
        vlcSettings.append(" --http-reconnect --http-continuous --no-video-title-show");
        #ifdef QT_DEBUG
        vlcSettings.append(" --extraintf=logger"); /* log anything */
        vlcSettings.append(" --verbose=2"); /* be much more verbose then normal for debugging purpose */
        #endif
        settings->setValue("vlcsettings", vlcSettings);
    }
    fontsize = settings->value("fontsize",32).toInt();
    minfontsize = settings->value("minfontsize",10).toInt();
    magnification = settings->value("magnification",1.0).toDouble();
    VideoWidget::setTextProperties(new PaintTextProperties("Courier New", fontsize, magnification));

    QStringList args = vlcSettings.split(" ");
    int num = args.length();
    char **argsForVLC = new char*[num];
    for(int i = 0; i < num; i++)
    {
        int len = args.at(i).length();
        char *word = new char [len + 1];
        for(int j = 0; j < len; j++)
            word[j] = args.at(i).at(j).toAscii();
        word[len] = '\0';
        argsForVLC[i] = word;
        vargusLog.writeToFile(argsForVLC[i]);
    }
    VideoWidget::setVlcArgs( argsForVLC, num);
    if(server == "" || port < 0 || port > 65535)
        return false;
    return true;
}

//change connection settings button clicked
void MainWindow::changeConnectionSettings()
{
    SettingsDialog d(this, server, port, t_server, t_port);
    connect(&d, SIGNAL(newSettings(QString, int, QString, int)),
            this, SLOT(newSettings(QString, int, QString, int)));
    d.exec();
}

void MainWindow::newSettings(QString newServer, int newPort, QString new_t_server, int new_t_port)
{
    if((server != newServer) || (port != newPort))
    {
        server = newServer;
        port = newPort;
        startConnection();
    }
    if((new_t_server != t_server) || (new_t_port != t_port))
    {
        t_server = new_t_server;
        t_port = new_t_port;
        Camera::changeTextServerSettings(t_server, t_port);
    }
    saveSettings();
}

void MainWindow::saveSettings()
{
    if(!settings) return;
    settings->setValue("server", server);
    settings->setValue("port", port);
    settings->setValue("t_server", t_server);
    settings->setValue("t_port", t_port);
    if(loggingEnabled)
        settings->setValue("logging", 1);
    else
        settings->setValue("logging", 0);
    settings->setValue("Directory for logs", pathForLogs);
}

void MainWindow::startConnection()
{
    disconnect(setTab, SIGNAL(currentChanged(int)), this, SLOT(onSetChanged(int)));
    foreach(Camera *c, camerasList)
        delete c;
    camList -> clear();
    camerasList.clear();
    foreach(View *v, viewsList)
        delete v;
    viewsList.clear();
    setTab->clear();
    foreach(Set *s, setsList)
        delete s;
    setsList.clear();
    // Обработка входных данных
    if(initData() == false)
        return;
    // Заполнение вкладок-сетов
    makeSets();
    if(set >= setsList.size())
        set = 0;
    setTab->setCurrentIndex(set);
    onSetChanged(set);
    connect(setTab, SIGNAL(currentChanged(int)), this, SLOT(onSetChanged(int)));
    changeActiveCameras(setsList.at(set)->getActiveCameras());
}

void MainWindow::enableButtons(bool prev, bool next)
{
    prevButton->setEnabled(prev);
    nextButton->setEnabled(next);
}

//make tray icon. If tray icon is clicked, apllication have to bicame invisible
void MainWindow::createIcons()
{
    QString imagePath = "/usr/share/vargus-viewer/images/";
    trIcon = new QSystemTrayIcon();
    trIcon->setIcon(QIcon(imagePath + "vargus32.png"));
    trIcon->show();
    connect(trIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(showHide(QSystemTrayIcon::ActivationReason)));
}

void MainWindow::showHide(QSystemTrayIcon::ActivationReason r) {
    if (r == QSystemTrayIcon::Trigger)  //left mouse button
    {
        vargusLog.writeToFile("Tray icon clicked (LMB)");
        if (!isVisible()) {
            show();
        } else {
            hide();
        }
    }
    else if(r == QSystemTrayIcon::Context)
    {
        vargusLog.writeToFile("Tray icon clicked (RMB)");
        contextMenu->show();
    }
}

//show dilog to change vlc settings
void MainWindow::vlcsettingsDialog()
{
    VLCSetingsDialog dialog(vlcSettings, this);
    connect(&dialog, SIGNAL(newSettings(QString&)), this, SLOT(newSettingsForVLC(QString&)));
    dialog.exec();
}

void MainWindow::newSettingsForVLC(QString &_vlcsettings)
{
    vlcSettings = _vlcsettings;
    settings->setValue("vlcsettings", vlcSettings);
}

//need to update camera. Will send requests to server.
void MainWindow::updateCamera(Camera *c)
{
    if(c == NULL)
        return;
    vargusLog.writeToFile(QString("Update camera %1").arg(c->name()));
    socket = new QAbstractSocket(QAbstractSocket::TcpSocket, this);
    socket->connectToHost(server, port);
    if(!socket->waitForConnected(5000))
    {
        QMessageBox::critical(NULL, tr("Error"), tr("Can not connect to server.\nPlease, change the connection settings."));
        delete socket;
        return;
    }
    socket->write(QString("query camera;%1;view:source,view:preview\n").arg(c->number()).toAscii());
    QStringList inf = readAnswer(1);
    QStringList adresses = inf.at(0).split(';');
    vargusLog.writeToFile(QString("New source %1").arg(adresses.at(0)));
    c->setSource(adresses.at(0));
    vargusLog.writeToFile(QString("New preview %1").arg(adresses.at(1).trimmed()));
    c->setPreview(adresses.at(1).trimmed());
    camList->updateCameraData(c);
    socket->write("exit\n");
    socket->disconnect();
    delete socket;
}

void MainWindow::logSettings()
{
    LogSettings ls;
    ls.exec();
}

void MainWindow::fontSettings()
{
    FontSettings fs(fontsize, minfontsize, magnification);
    connect(&fs, SIGNAL(fontchanged(int, int, double)), this, SLOT(fontChanged(int, int, double)));
    fs.exec();
}

void MainWindow::fontChanged(int newfsize, int newmfsize, double newmagnification)
{
    fontsize = newfsize;
    minfontsize = newmfsize;
    magnification = newmagnification;
    VideoWidget::setTextProperties(new PaintTextProperties("Courier New", fontsize, magnification));
    if(!settings)
        return;
    settings->setValue("fontsize", fontsize);
    settings->setValue("minfontsize", minfontsize);
    settings->setValue("magnification", magnification);
}

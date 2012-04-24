#include "settingsdialog.h"
#include<QHBoxLayout>
#include<QGridLayout>
#include<QRegExpValidator>
#include<QRegExp>

SettingsDialog::SettingsDialog(QWidget *parent, const QString &server,
                               int port, const QString &tserver, int tport) :
    QDialog(parent)
{
    setWindowTitle(tr("Connection settings"));
    serverLabel = new QLabel(tr("&Server adress"),this);
    portLabel = new QLabel(tr("&Port"),this);
    serverLineEdit = new QLineEdit(this);
    portLineEdit = new QLineEdit(this);
    connect(serverLineEdit,SIGNAL(textChanged(QString)),this,SLOT(enableOk()));
    connect(portLineEdit,SIGNAL(textChanged(QString)),this,SLOT(enableOk()));    
    serverLabel -> setBuddy(serverLineEdit);
    portLabel -> setBuddy(portLineEdit);

    t_serverLabel = new QLabel(tr("&Server adress"),this);
    t_portLabel = new QLabel(tr("&Port"),this);
    t_serverLineEdit = new QLineEdit(this);
    t_portLineEdit = new QLineEdit(this);
    connect(t_serverLineEdit,SIGNAL(textChanged(QString)),this,SLOT(enableOk()));
    connect(t_portLineEdit,SIGNAL(textChanged(QString)),this,SLOT(enableOk()));
    t_serverLabel -> setBuddy(t_serverLineEdit);
    t_portLabel -> setBuddy(t_portLineEdit);


    g1 = new QGroupBox(tr("Video connection setings"),this);
    g2 = new QGroupBox(tr("Text connection settings"), this);
    createButtons();
    createLayouts();
    setFixedHeight(250);
    QRegExp regExp("[0-9]{1,5}");
    portLineEdit -> setValidator(new QRegExpValidator(regExp));
    portLineEdit -> setValidator(new QRegExpValidator(regExp));
    serverLineEdit -> setText(server);
    if((port >= 0) && (port <= 65535))
        portLineEdit -> setText(QString::number(port));
    if(tserver == "")
        t_serverLineEdit -> setText(serverLineEdit -> text());
    else
        t_serverLineEdit -> setText(tserver);
    if(tport == 0)
        t_portLineEdit -> setText(portLineEdit -> text());
    else if( (tport > 0) && (tport <= 65535) )
        t_portLineEdit -> setText(QString::number(tport));

}

void SettingsDialog::createLayouts()
{
    QVBoxLayout *vl = new QVBoxLayout(this);
    QGridLayout *group1 = new QGridLayout(this);

    group1 -> addWidget(serverLabel, 0, 0);
    group1 -> addWidget(serverLineEdit, 0, 1);
    group1 -> addWidget(portLabel, 1, 0);
    group1 -> addWidget(portLineEdit, 1, 1);
    g1 -> setLayout(group1);
    vl -> addWidget(g1);

    QGridLayout *group2 = new QGridLayout(this);
    group2 -> addWidget(t_serverLabel, 0, 0);
    group2 -> addWidget(t_serverLineEdit, 0, 1);
    group2 -> addWidget(t_portLabel, 1, 0);
    group2 -> addWidget(t_portLineEdit, 1, 1);
    g2 -> setLayout(group2);
    vl -> addWidget(g2);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout -> addWidget(cloneButton);
    buttonLayout -> addStretch(100);
    buttonLayout -> addWidget(okButton);
    buttonLayout -> addWidget(cancelButton);
    vl -> addLayout(buttonLayout);
}

void SettingsDialog::createButtons()
{
    okButton = new QPushButton(tr("&OK"), this);
    okButton -> setEnabled(false);
    cancelButton = new QPushButton(tr("&Cancel"), this);
    cloneButton = new QPushButton(tr("&Clone"), this);
    connect(okButton,SIGNAL(clicked()),this,SLOT(okButtonClicked()));
    connect(cancelButton,SIGNAL(clicked()),this,SLOT(close()));
    connect(cloneButton, SIGNAL(clicked()), this, SLOT(clone()));
}

void SettingsDialog::okButtonClicked()
{
    emit newSettings(serverLineEdit -> text(), portLineEdit -> text().toInt(),
                     t_serverLineEdit -> text(), t_portLineEdit -> text().toInt());
    close();
}

void SettingsDialog::enableOk()
{
    okButton -> setEnabled(!(serverLineEdit -> text().isEmpty())
                           && !(portLineEdit -> text().isEmpty())
                           && !(t_serverLineEdit -> text().isEmpty())
                           && !(t_portLineEdit -> text().isEmpty()));
}

SettingsDialog::~SettingsDialog()
{
    delete serverLabel;
    delete t_serverLabel;
    delete portLabel;
    delete t_portLabel;
    delete serverLineEdit;
    delete t_serverLineEdit;
    delete portLineEdit;
    delete t_portLineEdit;
    delete okButton;
    delete cancelButton;
    delete cloneButton;
    delete g1;
    delete g2;
}

void SettingsDialog::clone()
{
    t_serverLineEdit -> setText(serverLineEdit -> text());
    t_portLineEdit -> setText(portLineEdit -> text());
}

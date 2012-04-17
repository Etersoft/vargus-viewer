#include "settingsdialog.h"
#include<QHBoxLayout>
#include<QGridLayout>
#include<QRegExpValidator>
#include<QRegExp>

SettingsDialog::SettingsDialog(QWidget *parent, QString server, int port) :
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
    createButtons();
    createLayouts();
    int h = okButton->size().height();
    int w = okButton->size().width();
    okButton -> setMinimumSize(w,h);
    okButton -> setMaximumSize(w,h);
    cancelButton -> setMinimumSize(w,h);
    cancelButton -> setMaximumSize(w,h);
    setFixedHeight(100);
    QRegExp regExp("[0-9]{1,5}");
    portLineEdit -> setValidator(new QRegExpValidator(regExp));
    serverLineEdit -> setText(server);
    if((port >= 0) && (port <= 65535))
        portLineEdit -> setText(QString::number(port));
}

void SettingsDialog::createLayouts()
{
    QGridLayout *grid = new QGridLayout(this);
    grid -> addWidget(serverLabel,0,0);
    grid -> addWidget(serverLineEdit,0,1);
    grid -> addWidget(portLabel,1,0);
    grid -> addWidget(portLineEdit,1,1);
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout -> addWidget(okButton);
    buttonLayout -> addWidget(cancelButton);
    grid -> addLayout(buttonLayout,2,1);
}

void SettingsDialog::createButtons()
{
    okButton = new QPushButton(this);
    okButton -> setText(tr("&Ok"));
    okButton -> setEnabled(false);
    cancelButton = new QPushButton(this);
    cancelButton -> setText(tr("&Cancel"));
    connect(okButton,SIGNAL(clicked()),this,SLOT(okButtonClicked()));
    connect(cancelButton,SIGNAL(clicked()),this,SLOT(close()));
}

void SettingsDialog::okButtonClicked()
{
    emit newSettings(serverLineEdit -> text(), portLineEdit -> text().toInt());
    close();
}

void SettingsDialog::enableOk()
{
    okButton -> setEnabled(!(serverLineEdit -> text().isEmpty()) && !(portLineEdit -> text().isEmpty()));
}

SettingsDialog::~SettingsDialog()
{
    delete serverLabel;
    delete portLabel;
    delete serverLineEdit;
    delete portLineEdit;
    delete okButton;
    delete cancelButton;
}

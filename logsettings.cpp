/*
Menu for log settings.

Copyright (C) 2012-2013 Etersoft <info@etersoft.ru>

This file is part of VargusViewer.

VargusViewer is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

VargusViewer is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#include "logsettings.h"
#include <QPushButton>
#include <QGridLayout>
#include <QSettings>
#include <QMessageBox>
#include "logger.h"
#include <QFileDialog>

extern Logger &vargusLog;

LogSettings::LogSettings(QWidget *parent) :
    QDialog(parent)
{
    pathButton = new QPushButton(tr("Path for logs"));
    connect(pathButton, SIGNAL(clicked()), this, SLOT(changePath()));
    defaultButton = new QPushButton(tr("Default path for logs"));
    connect(defaultButton, SIGNAL(clicked()), this, SLOT(setDefaultPath()));
    deleteButton = new QPushButton(tr("Remove log files"));
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(removeLogs()));
    enableCheckBox = new QCheckBox(tr("Enable logs"));
    pathLine = new QLineEdit();

    applyButton = new QPushButton(tr("Apply"));
    connect(applyButton, SIGNAL(clicked()), this, SLOT(applyClicked()));
    cancelButton = new QPushButton(tr("Cancel"));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

    QGridLayout *grid = new QGridLayout(this);
    grid->addWidget(enableCheckBox, 0, 0);
    grid->addWidget(deleteButton, 0, 2);
    grid->addWidget(pathLine, 1, 0, 1, 3);
    grid->addWidget(pathButton, 2, 1);
    grid->addWidget(defaultButton, 2, 2);
    grid->addWidget(cancelButton, 3, 1);
    grid->addWidget(applyButton, 3, 2);
    readSettings();
}

void LogSettings::readSettings()
{
    QSettings settings("Etersoft","VargusViewer");
    int t = settings.value("logging", -1).toInt();
    if(t == 1 || t == -1)
        enableCheckBox->setChecked(true);
    else enableCheckBox->setChecked(false);
    QString pathForLogs = settings.value("Directory for logs","").toString();
    if(pathForLogs == "")
    {
        pathForLogs = WORKDIR;
        pathForLogs += "logs/";
    }
    pathLine->setText(pathForLogs);
}

void LogSettings::removeLogs()
{
    vargusLog.writeToFile("Action delete other log files clicked");
    if( vargusLog.deleteLogFiles() )
    {
        QMessageBox::information(this, tr("Complete"), tr("Log files are deleted."));
    }
    else
        QMessageBox::information(this, tr("Unable to process"), tr("Log files are not deleted."));
}

void LogSettings::setDefaultPath()
{
    QString pathForLogs = WORKDIR;
    pathForLogs += "logs/";
    pathLine->setText(pathForLogs);
}

void LogSettings::changePath()
{
    vargusLog.writeToFile("Change log file directory");
    QString newPath = QFileDialog::getExistingDirectory(this, tr("Chose directory for saving log files."));
    vargusLog.writeToFile("Change log file directory to " + newPath);
    if(!newPath.isEmpty())
    {
        pathLine->setText(newPath);
    }
}

void LogSettings::applyClicked()
{
    QSettings settings("Etersoft","VargusViewer");
    if(vargusLog.changeDirectory(pathLine->text()))
    {
        settings.setValue("Directory for logs", pathLine->text());
    }
    if(enableCheckBox->isChecked())
    {
        vargusLog.setActive(true);
        settings.setValue("logging", 1);
    }
    else
    {
        vargusLog.setActive(false);
        settings.setValue("logging", 0);
    }
    this->accept();
}

LogSettings::~LogSettings()
{
    delete pathButton;
    delete defaultButton;
    delete deleteButton;
    delete enableCheckBox;
    delete applyButton;
    delete cancelButton;
}

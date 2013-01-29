/*
Menu for vlc settings.

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

#include "vlcsettingsdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>


VLCSetingsDialog::VLCSetingsDialog(const QString &currentSettings, QWidget *parent) :
    QDialog(parent)
{
    okButton = new QPushButton(tr("OK"));
    connect(okButton, SIGNAL(clicked()), this, SLOT(loadNewSettings()));
    cancelButton = new QPushButton(tr("Cancel"));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
    defaultButton = new QPushButton(tr("Default"));
    connect(defaultButton, SIGNAL(clicked()), this, SLOT(uploadDefault()));
    textEdit = new QTextEdit();
    QString labelText = tr("Input new vlc settings here.");
    labelText.append("\n");
    labelText.append(tr("New settings will be used after restart."));
    label = new QLabel(labelText);
    QVBoxLayout *l = new QVBoxLayout(this);
    l->addWidget(label);
    l->addWidget(textEdit);
    QHBoxLayout *buttons = new QHBoxLayout();
    buttons->addWidget(okButton);
    buttons->addWidget(cancelButton);
    buttons->addWidget(defaultButton);
    l->addLayout(buttons);
    vlc_args.append("-I dummy"); /* Don't use any interface */
    vlc_args.append(" --ignore-config"); /* Don't use VLC's config */
    vlc_args.append(" --no-audio"); /* Audio off */
    vlc_args.append(" --http-reconnect --http-continuous --no-video-title-show");
#ifdef QT_DEBUG
    vlc_args.append(" --extraintf=logger"); /* log anything */
    vlc_args.append(" --verbose=2"); /* be much more verbose then normal for debugging purpose */
#endif
    textEdit->setText(currentSettings);
}

void VLCSetingsDialog::uploadDefault()
{
    textEdit->clear();
    textEdit->setText(vlc_args);
}

void VLCSetingsDialog::loadNewSettings()
{
    QString settings = textEdit->toPlainText();
    QString settingsToEmit;
    for(int i = 0; i < settings.size(); i++)
    {
        if(settings.at(i) == '\n')
            settingsToEmit.append(" ");
        else
            settingsToEmit.append(settings.at(i));
    }
    emit newSettings(settingsToEmit);
    accept();
}

VLCSetingsDialog::~VLCSetingsDialog()
{
    delete okButton;
    delete cancelButton;
    delete defaultButton;
    delete textEdit;
    delete label;
}

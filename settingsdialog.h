/*
Menu for conection settings.

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

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGroupBox>

class SettingsDialog : public QDialog
{
    Q_OBJECT

    QLabel *serverLabel;
    QLabel *portLabel;
    QLineEdit *serverLineEdit;
    QLineEdit *portLineEdit;
    QPushButton *okButton;
    QPushButton *cancelButton;
    QPushButton *cloneButton;
    QGroupBox *g1;
    QGroupBox *g2;
    QLabel *t_serverLabel;
    QLabel *t_portLabel;
    QLineEdit *t_serverLineEdit;
    QLineEdit *t_portLineEdit;

    void createLayouts();
    void createButtons();

 private slots:
    void okButtonClicked();
    void enableButtons();
    void clone();

public:
    explicit SettingsDialog(QWidget *parent = 0,
                            const QString &server = "",
                            int port = 0, const QString &tserver = "",
                            int tport = 0);
    ~SettingsDialog();
    
signals:
    void newSettings(QString server, int port, QString t_server, int t_port);
};

#endif // SETTINGSDIALOG_H

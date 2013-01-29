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

#ifndef VLCSETTINGSDIALOG_H
#define VLCSETTINGSDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>

class VLCSetingsDialog : public QDialog
{
    Q_OBJECT

    QPushButton *okButton;
    QPushButton *cancelButton;
    QPushButton *defaultButton;
    QTextEdit *textEdit;
    QLabel *label;

    QString vlc_args;

public:
    explicit VLCSetingsDialog(const QString &currentSettings, QWidget *parent = 0);
    ~VLCSetingsDialog();
    
signals:
    void newSettings(QString &settings);

private slots:
    void uploadDefault();
    void loadNewSettings();
};

#endif // VLCSETTINGSDIALOG_H

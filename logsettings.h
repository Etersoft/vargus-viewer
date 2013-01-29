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

#ifndef LOGSETTINGS_H
#define LOGSETTINGS_H

#include <QDialog>
#include <QCheckBox>
#include <QLineEdit>

class LogSettings : public QDialog
{
    Q_OBJECT

    QPushButton *pathButton;
    QPushButton *defaultButton;
    QPushButton *deleteButton;
    QCheckBox *enableCheckBox;
    QLineEdit *pathLine;

    QPushButton *applyButton;
    QPushButton *cancelButton;

public:
    explicit LogSettings(QWidget *parent = 0);
    ~LogSettings();

private:
    void readSettings();

private slots:
    void setDefaultPath();
    void removeLogs();
    void changePath();
    void applyClicked();
    
public slots:
    
};

#endif // LOGSETTINGS_H

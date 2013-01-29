/*


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

#ifndef FONTSETTINGS_H
#define FONTSETTINGS_H

#include <QDialog>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class FontSettings : public QDialog
{
    Q_OBJECT

    QSpinBox *fontsize;
    QDoubleSpinBox *magnification;
    QLineEdit *fonttype;
    QLabel *fslabel;
    QLabel *fontlabel;
    QLabel *mlabel;
    QPushButton *okButton;
    QPushButton *cancelButton;
    int fsize;
    double magn;
    QString ftype;
public:
    explicit FontSettings(int _fsize, double _magnification, QString _font, QWidget *parent = 0);
    ~FontSettings();
    
signals:
    void fontchanged(int size, double magnification, QString font);
    
private slots:
    void process();
    
};

#endif // FONTSETTINGS_H

#ifndef FONTSETTINGS_H
#define FONTSETTINGS_H

#include <QDialog>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QPushButton>

class FontSettings : public QDialog
{
    Q_OBJECT

    QSpinBox *fontsize;
    QDoubleSpinBox *magnification;
    QLabel *fslabel;
    QLabel *mlabel;
    QPushButton *okButton;
    QPushButton *cancelButton;
    int fsize;
    double magn;
public:
    explicit FontSettings(int _fsize, double _magnification, QWidget *parent = 0);
    ~FontSettings();
    
signals:
    void fontchanged(int size, double magnification);
    
private slots:
    void process();
    
};

#endif // FONTSETTINGS_H

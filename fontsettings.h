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
    QSpinBox *minfontsize;
    QDoubleSpinBox *magnification;
    QLabel *fslabel;
    QLabel *minsizelabel;
    QLabel *mlabel;
    QPushButton *okButton;
    QPushButton *cancelButton;
    int fsize;
    int mfsize;
    double magn;
public:
    explicit FontSettings(int _fsize, int _mfsize, double _magnification, QWidget *parent = 0);
    ~FontSettings();
    
signals:
    void fontchanged(int size, int minsize, double magnification);
    
private slots:
    void process();
    
};

#endif // FONTSETTINGS_H

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

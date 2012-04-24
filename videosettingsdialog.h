#ifndef VIDEOSETTINGSDIALOG_H
#define VIDEOSETTINGSDIALOG_H

#include <QDialog>
#include <QRadioButton>
#include <QPushButton>
#include <QGroupBox>
#include<enums.h>

class VideoSettingsDialog : public QDialog
{
    Q_OBJECT
private:
    QRadioButton *b1;
    QRadioButton *b2;
    QPushButton *buttonOk;
    QPushButton *buttonCancel;
    QGroupBox *g;

    void makeButtons();
    void createLayouts();
public:
    explicit VideoSettingsDialog(VPlayingType t, QWidget *parent = 0);
    ~VideoSettingsDialog();
    
signals:
    void settingsChanged(VPlayingType);
    
private slots:
    void okClicked();
    
};

#endif // VIDEOSETTINGSDIALOG_H

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

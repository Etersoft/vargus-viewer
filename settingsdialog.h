#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include<QPushButton>

class SettingsDialog : public QDialog
{
    Q_OBJECT
private:
    QLabel *serverLabel;
    QLabel *portLabel;
    QLineEdit *serverLineEdit;
    QLineEdit *portLineEdit;
    QPushButton *okButton;
    QPushButton *cancelButton;

    void createLayouts();
    void createButtons();

 private slots:
    void okButtonClicked();
    void enableOk();
public:
    explicit SettingsDialog(QWidget *parent = 0,QString server = "",int port = 0);
    ~SettingsDialog();
    
signals:
    void newSettings(QString server, int port);
    
public slots:
    
};

#endif // SETTINGSDIALOG_H

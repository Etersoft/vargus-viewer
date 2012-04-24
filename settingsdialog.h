#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include<QPushButton>
#include<QGroupBox>

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
    void enableOk();
    void clone();
public:
    explicit SettingsDialog(QWidget *parent = 0,
                            const QString &server = "",
                            int port = 0, const QString &tserver = "",
                            int tport = 0);
    ~SettingsDialog();
    
signals:
    void newSettings(QString server, int port, QString t_server, int t_port);
    
public slots:
    
};

#endif // SETTINGSDIALOG_H

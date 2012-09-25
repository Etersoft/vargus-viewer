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

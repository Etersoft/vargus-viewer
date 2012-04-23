#include "videosettingsdialog.h"
#include <QVBoxLayout>
#include<QGroupBox>
#include<QRadioButton>
VideoSettingsDialog::VideoSettingsDialog(QWidget *parent) :
    QDialog(parent)
{
    QVBoxLayout *l = new QVBoxLayout(this);
    QGroupBox *g = new QGroupBox(tr("Way of playing"),this);
    QRadioButton *b1 = new QRadioButton(tr("&X-window"),this);
    QRadioButton *b2 = new QRadioButton(tr("&OpenGL"),this);
    QVBoxLayout *groupLayout = new QVBoxLayout(this);
    groupLayout -> addWidget(b1);
    groupLayout -> addWidget(b2);
    g->setLayout(groupLayout);
    l -> addWidget(g);

}

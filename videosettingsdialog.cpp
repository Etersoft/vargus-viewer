#include "videosettingsdialog.h"
#include <QVBoxLayout>
VideoSettingsDialog::VideoSettingsDialog(VPlayingType t, QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("Video settings"));
    g = new QGroupBox(tr("Way of playing"), this);
    makeButtons();
    createLayouts();
    if(t == XWINDOW)
        b1->setChecked(true);
    else
        b2->setChecked(true);
    setFixedSize(180, 150);
}

void VideoSettingsDialog::makeButtons()
{
    b1 = new QRadioButton(tr("&X Window"), this);
    b2 = new QRadioButton(tr("&Low-level painting"), this);

    buttonOk = new QPushButton(tr("&OK"), this);
    buttonCancel = new QPushButton(tr("&Cancel"), this);
    connect(buttonOk, SIGNAL(clicked()), this, SLOT(okClicked()));
    connect(buttonCancel, SIGNAL(clicked()), this, SLOT(close()));
}

void VideoSettingsDialog::createLayouts()
{
    QVBoxLayout *l = new QVBoxLayout(this);
    QVBoxLayout *groupLayout = new QVBoxLayout(this);
    QHBoxLayout *buttonsLayout = new QHBoxLayout(this);
    groupLayout->addWidget(b1);
    groupLayout->addWidget(b2);
    g->setLayout(groupLayout);
    l->addWidget(g);
    buttonsLayout->addWidget(buttonOk);
    buttonsLayout->addWidget(buttonCancel);
    l->addLayout(buttonsLayout);
}

void VideoSettingsDialog::okClicked()
{
    if(b1->isChecked())
        emit settingsChanged(XWINDOW);
    else
        emit settingsChanged(LOWLEVEL);
    close();
}

VideoSettingsDialog::~VideoSettingsDialog()
{
    delete b1;
    delete b2;
    delete g;
    delete buttonOk;
    delete buttonCancel;
}

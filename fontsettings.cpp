#include "fontsettings.h"
#include <QGridLayout>

FontSettings::FontSettings(int _fsize, int _mfsize, double _magnification, QWidget *parent) :
    QDialog(parent)
{
    fsize = _fsize;
    mfsize = _mfsize;
    magn = _magnification;

    fslabel = new QLabel(tr("Prefered font size:"));
    fontsize = new QSpinBox();
    fontsize->setRange(1, 100);// toStas кпределы
    fontsize->setValue(_fsize);
    fslabel->setBuddy(fontsize);

    minsizelabel = new QLabel(tr("Minimal font size:"));
    minfontsize = new QSpinBox();
    minfontsize->setRange(1, 100);// toStas кпределы
    minfontsize->setValue(_mfsize);
    minsizelabel->setBuddy(fontsize);

    mlabel = new QLabel(tr("Magnification:"));
    magnification = new QDoubleSpinBox();
    magnification->setRange(0,10); // toStas пределы
    magnification->setValue(_magnification);
    magnification->setSingleStep(0.1); // toStas какой шаг изменения
    mlabel->setBuddy(magnification);

    okButton = new QPushButton(tr("OK"));
    cancelButton = new QPushButton(tr("Cancel"));

    QGridLayout *grid = new QGridLayout(this);
    grid->addWidget(fslabel, 0, 0);
    grid->addWidget(fontsize, 0, 1);
    grid->addWidget(minsizelabel, 1, 0);
    grid->addWidget(minfontsize, 1, 1);
    grid->addWidget(mlabel, 2, 0);
    grid->addWidget(magnification, 2, 1);
    grid->addWidget(okButton, 3, 0);
    grid->addWidget(cancelButton, 3, 1);

    connect(okButton, SIGNAL(clicked()), this, SLOT(process()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}

FontSettings::~FontSettings()
{
    delete fslabel;
    delete minsizelabel;
    delete fontsize;
    delete minfontsize;
    delete mlabel;
    delete magnification;
    delete okButton;
    delete cancelButton;
}

void FontSettings::process()
{
    if(fontsize->value() != fsize || minfontsize->value() != mfsize || magnification->value() != magn)
        emit fontchanged(fontsize->value(), minfontsize->value(), magnification->value());
    accept();
}

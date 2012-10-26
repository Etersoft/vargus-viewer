#include "fontsettings.h"
#include <QGridLayout>

FontSettings::FontSettings(int _fsize, double _magnification, QWidget *parent) :
    QDialog(parent)
{
    fsize = _fsize;
    magn = _magnification;

    fslabel = new QLabel(tr("Font size:"));
    fontsize = new QSpinBox();
    fontsize->setRange(1,100);// toStas кпределы
    fontsize->setValue(_fsize);
    fslabel->setBuddy(fontsize);

    mlabel = new QLabel(tr("Magnification:"));
    magnification = new QDoubleSpinBox();
    magnification->setRange(1,100); // toStas пределы
    magnification->setValue(_magnification);
    magnification->setSingleStep(0.1); // toStas какой шаг изменения
    mlabel->setBuddy(magnification);

    okButton = new QPushButton(tr("OK"));
    cancelButton = new QPushButton(tr("Cancel"));

    QGridLayout *grid = new QGridLayout(this);
    grid->addWidget(fslabel,0,0);
    grid->addWidget(fontsize,0,1);
    grid->addWidget(mlabel,1,0);
    grid->addWidget(magnification,1,1);
    grid->addWidget(okButton,2,0);
    grid->addWidget(cancelButton,2,1);

    connect(okButton, SIGNAL(clicked()), this, SLOT(process()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}

FontSettings::~FontSettings()
{
    delete fslabel;
    delete fontsize;
    delete mlabel;
    delete magnification;
    delete okButton;
    delete cancelButton;
}

void FontSettings::process()
{
    if(fontsize->value() != fsize || magnification->value() != magn)
        emit fontchanged(fontsize->value(), magnification->value());
    accept();
}

#include "fontsettings.h"
#include <QGridLayout>

FontSettings::FontSettings(int _fsize, double _magnification, QString _font,  QWidget *parent) :
    QDialog(parent)
{
    fsize = _fsize;
    magn = _magnification;
    ftype = _font;

    fslabel = new QLabel(tr("Prefered font size:"));
    fontsize = new QSpinBox();
    fontsize->setRange(1, 100);// toStas кпределы
    fontsize->setValue(_fsize);
    fslabel->setBuddy(fontsize);

    mlabel = new QLabel(tr("Magnification:"));
    magnification = new QDoubleSpinBox();
    magnification->setRange(0,10); // toStas пределы
    magnification->setValue(_magnification);
    magnification->setSingleStep(0.1); // toStas какой шаг изменения
    mlabel->setBuddy(magnification);

    fontlabel = new QLabel(tr("Font:"));
    fonttype = new QLineEdit(ftype);

    okButton = new QPushButton(tr("OK"));
    cancelButton = new QPushButton(tr("Cancel"));

    QGridLayout *grid = new QGridLayout(this);
    grid->addWidget(fslabel, 0, 0);
    grid->addWidget(fontsize, 0, 1);
    grid->addWidget(mlabel, 1, 0);
    grid->addWidget(magnification, 1, 1);
    grid->addWidget(fontlabel, 2, 0);
    grid->addWidget(fonttype, 2, 1);
    grid->addWidget(okButton, 3, 0);
    grid->addWidget(cancelButton, 3, 1);

    connect(okButton, SIGNAL(clicked()), this, SLOT(process()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}

FontSettings::~FontSettings()
{
    delete fslabel;
    delete fontsize;
    delete mlabel;
    delete magnification;
    delete fontlabel;
    delete fonttype;
    delete okButton;
    delete cancelButton;
}

void FontSettings::process()
{
    if(fontsize->value() != fsize || magnification->value() != magn || fonttype->text() != ftype)
        emit fontchanged(fontsize->value(), magnification->value(), fonttype->text());
    accept();
}

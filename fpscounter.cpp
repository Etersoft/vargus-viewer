#include "fpscounter.h"
#include"camera.h"

FPSCounter::FPSCounter(QWidget *parent) :
    QDialog(parent)
{
    table = NULL;
    grid = new QGridLayout(this);
    updateButton =new QPushButton(tr("&Обновить"),this);
    grid -> addWidget(updateButton,1,1);
    connect(updateButton, SIGNAL(clicked()), this, SLOT(update()));

}

void FPSCounter::start()
{
    if(table)
        delete table;
    table = new QTableWidget(video.length(),2,this);
    grid->addWidget(table,0,0);
    setLayout(grid);

    QList<VideoWidget *>::iterator it = video.begin();
    for(int i = 0; i < video.length(); i++, it++)
    {
        Camera *c = (*it) -> getCamera();
        QTableWidgetItem *item = new QTableWidgetItem();
        if(c)
            item->setText(c -> description());
        table -> setItem(i, 0, item);
        item = new QTableWidgetItem();
        item -> setText(QString::number((*it) -> getFPS()));
        table -> setItem(i, 1, item);
    }
}

void FPSCounter::update()
{
    QList<VideoWidget *>::iterator it = video.begin();
    for(int i = 0; i < video.length(); i++, it++)
    {
        table -> item(i,1) -> setText(QString::number((*it) -> getFPS()));
    }
}

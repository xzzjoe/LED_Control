
#include "plusminus.h"
#include <QGridLayout>

PlusMinus::PlusMinus(QWidget *parent)
    : QWidget(parent) {
    val = 0;
    auto *plsBtn = new QPushButton("Increase Brightness", this);
    auto *minBtn = new QPushButton("Decrease Brightness", this);
    lbl = new QLabel("Brightness: " + QString::number(val) + "%", this);
    bar = new QProgressBar(this);
    bar->setValue(val);
    bar->setOrientation(Qt::Vertical);
    auto *grid = new QGridLayout(this);
    grid->addWidget(plsBtn, 0, 0, 2, 1);
    grid->addWidget(minBtn, 2, 0, 2, 1);
    grid->addWidget(lbl, 0, 1,Qt::AlignCenter);
    grid->addWidget(bar,1,1,3,1,Qt::AlignCenter);
    //bar->setGeometry(300,50,75,200);
//    plsBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//    minBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    plsBtn->setMinimumSize(200,100);
    minBtn->setMinimumSize(200,100);
    bar->setMinimumSize(75,200);


//    grid->setColumnMinimumWidth(1,70);
//    grid->setColumnMinimumWidth(2,70);

    setLayout(grid);

    connect(plsBtn, &QPushButton::clicked, this, &PlusMinus::OnPlus);
    connect(minBtn, &QPushButton::clicked, this, &PlusMinus::OnMinus);
}

void PlusMinus::OnPlus() {

    if(val+brightIncr<=100) val+=brightIncr;
    lbl->setText("Brightness: " + QString::number(val) + "%");
    bar->setValue(val);
}

void PlusMinus::OnMinus() {

    if(val-brightIncr>=0)val-=brightIncr;
    lbl->setText("Brightness: " + QString::number(val) + "%");
    bar->setValue(val);
}


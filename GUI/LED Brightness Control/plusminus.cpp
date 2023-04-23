
#include "plusminus.h"
#include <QGridLayout>

PlusMinus::PlusMinus(QWidget *parent)
    : QWidget(parent) {

    auto *plsBtn = new QPushButton("Increase Brightness", this);
    auto *minBtn = new QPushButton("Decrease Brightness", this);
    lbl = new QLabel("0", this);
    bright = new QLabel("Brightness(0-100):", this);

    auto *grid = new QGridLayout(this);
    grid->addWidget(plsBtn, 0, 0);
    grid->addWidget(minBtn, 2, 0);
    grid->addWidget(bright, 1, 1);
    grid->addWidget(lbl, 1, 2);

    setLayout(grid);

    connect(plsBtn, &QPushButton::clicked, this, &PlusMinus::OnPlus);
    connect(minBtn, &QPushButton::clicked, this, &PlusMinus::OnMinus);
}

void PlusMinus::OnPlus() {

    int val = lbl->text().toInt();
    if(val<100) val++;
    lbl->setText(QString::number(val));
}

void PlusMinus::OnMinus() {

    int val = lbl->text().toInt();
    if(val>0)val--;
    lbl->setText(QString::number(val));
}


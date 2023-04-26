//Credit to https://zetcode.com/gui/qt5/firstprograms/ for the initial button plus minus code
#include "plusminus.h"
#include <QGridLayout>
PlusMinus::PlusMinus(QWidget *parent)
    : QWidget(parent) {
    watcher.addPath ("/sys/class/gpio/gpio66/value");
    QObject::connect(&watcher, SIGNAL (fileChanged(QString)),\
                         this, SLOT   (switchChanged(QString)));
    val = 0;
    numTimers=0;
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
    updateVal();
}

void PlusMinus::OnMinus() {

    if(val-brightIncr>=0)val-=brightIncr;
    updateVal();
}

void PlusMinus::switchChanged(QString str){
    Q_UNUSED(str); 
    update_val();   
    //TODO: logic for increasing brightness to the preset value
}

void PlusMinus::updateVal(){
    lbl->setText("Brightness: " + QString::number(val) + "%");
    bar->setValue(val);
    int dutyCycle = val*1000;
    QProcess::startDetached("/root/change.sh", QStringList {QString::number(dutyCycle)});
    //Every time updateVal is called, a single shot time out timer is created. Since they'll
    //overlap, and we only want the lights to turn off after the last timer expires, we keep
    //track of the number of timers and only turn off the lights once no timers are left.
    numTimers++;
    QTimer::singleShot(timeOutDuration*1000,this,SLOT(OnTimeOut()));

}
void PlusMinus::OnTimeOut(){
    numTimers--;
    if(numTimers==0) QProcess::startDetached("/root/change.sh", QStringList {QString::number(0)});
}


//Credit to https://zetcode.com/gui/qt5/firstprograms/ for the initial button plus minus code
//final
#include "plusminus.h"
#include <QGridLayout>
#include <QFileSystemWatcher>
#include <QTimer>
#include <unistd.h>
#include <poll.h>
#include <fcntl.h>

PlusMinus::PlusMinus(QWidget *parent)
    : QWidget(parent) {
    // watcher = new QFileSystemWatcher();
    // bool beingWatched = watcher->addPath("/sys/devices/platform/ocp/481ac000.gpio/gpiochip2/gpio/gpio66/value");
    // QObject::connect(watcher, SIGNAL(fileChanged(QString)),
    //                      this, SLOT(switchChanged(QString)));
    
    

    val = 0;
    numTimers=0;
    count = 0;
    off_flag = false;
    auto *plsBtn = new QPushButton("Brightness\n+", this);
    auto *minBtn = new QPushButton("Brightness\n-", this);
    auto *closeBtn = new QPushButton("Exit\nProgram", this);
    auto *off = new QPushButton("Light\nOff", this);
    auto *on = new QPushButton("Light\nOn", this);
    auto *timeDown = new QPushButton("Standby\n-", this);
    auto *timeUp = new QPushButton("Standby\n+", this);
    lbl = new QLabel(QString::number(val) + "%", this);
    tlbl = new QLabel(QString::number(timeOutDuration) + "s", this);
    bar = new QProgressBar(this);
    bar->setValue(val);
    bar->setOrientation(Qt::Vertical);
    auto *grid = new QGridLayout(this);
    grid->addWidget(plsBtn, 0, 1, 1, 1);
    grid->addWidget(minBtn, 2, 1, 1, 1);
    grid->addWidget(closeBtn, 2, 0);
    grid->addWidget(lbl, 1, 1,Qt::AlignCenter);
    grid->addWidget(tlbl, 1, 2,Qt::AlignCenter);
    grid->addWidget(bar,0,3,3,1,Qt::AlignCenter);
    grid->addWidget(off, 1, 0);
    grid->addWidget(on, 0, 0);
    grid->addWidget(timeDown, 2, 2);
    grid->addWidget(timeUp, 0, 2);
    //bar->setGeometry(300,50,75,200);
//    plsBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//    minBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    plsBtn->setMinimumSize(100,50);
    minBtn->setMinimumSize(100,50);
    closeBtn->setMinimumSize(50,25);
    bar->setMinimumSize(75,200);
    off->setMinimumSize(25, 25);
    on->setMinimumSize(25, 25);

    //Coloring Stuff
    on->setStyleSheet("QPushButton {background:rgb(230, 226, 122)}");
    off->setStyleSheet("QPushButton {background:rgb(230, 226, 122)}");
    closeBtn->setStyleSheet("QPushButton {background:rgb(222, 126, 115)}");
    plsBtn->setStyleSheet("QPushButton {background:rgb(84, 222, 240)}");
    minBtn->setStyleSheet("QPushButton {background:rgb(84, 222, 240)}");
    timeDown->setStyleSheet("QPushButton {background:rgb(110, 222, 104)}");
    timeUp->setStyleSheet("QPushButton {background:rgb(110, 222, 104)}");


//grid->setColumnMinimumWidth(3,150);
//    grid->setColumnMinimumWidth(2,70);

    setLayout(grid);

    connect(plsBtn, &QPushButton::clicked, this, &PlusMinus::OnPlus);
    connect(minBtn, &QPushButton::clicked, this, &PlusMinus::OnMinus);
    connect(closeBtn, &QPushButton::clicked, this, &PlusMinus::exit);
    connect(off, &QPushButton::clicked, this, &PlusMinus::turnOff);
    connect(on, &QPushButton::clicked, this, &PlusMinus::turnOn);
    connect(timeDown, &QPushButton::clicked, this, &PlusMinus::decrTimeOut);
    connect(timeUp, &QPushButton::clicked, this, &PlusMinus::incrTimeOut);
    
    timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(switchChanged()));
    timer->start(1000);
}

void PlusMinus::OnPlus() {

    if(val+brightIncr<=100) val+=brightIncr;
    updateVal();
}

void PlusMinus::OnMinus() {

    if(val-brightIncr>=0)val-=brightIncr;
    updateVal();
}

void PlusMinus::switchChanged(){
    int fd = open("/sys/class/gpio/gpio66/value",O_RDONLY);
    struct pollfd pfd;
    pfd.fd = fd;
    pfd.events = POLLIN;
    int pollRet = poll(&pfd, 1, 0);
    if (pollRet > 0) {
        if (pfd.revents & POLLIN) {
            int value = readFileValue(fd);\
            if(value == 1 && !off_flag) updateVal(); 

        }
    }
    count ++;
    ::close(fd);
}

void PlusMinus::updateVal(){
    lbl->setText(QString::number(val) + "%");
    bar->setValue(val);
    int dutyCycle = val*1000;
    QProcess::startDetached("/root/change.sh", QStringList {QString::number(dutyCycle)});
    //Every time updateVal is called, a single shot time out timer is created. Since they'll
    //overlap, and we only want the lights to turn off after the last timer expires, we keep
    //track of the number of timers and only turn off the lights once no timers are left.
    if(timeOutDuration>0){
        numTimers++;
        QTimer::singleShot(timeOutDuration*1000,this,SLOT(OnTimeOut()));
    }

}
void PlusMinus::OnTimeOut(){
    numTimers--;
    if((numTimers==0)&&(timeOutDuration>0)) QProcess::startDetached("/root/change.sh", QStringList {QString::number(0)});
}

void PlusMinus::exit(){
    QProcess::startDetached("/root/exit.sh", QStringList{});
    QApplication::exit();
}


void PlusMinus::turnOff(){
    lbl->setText(QString::number(0) + "%");
    bar->setValue(0);
    QProcess::startDetached("/root/change.sh", QStringList {QString::number(0)});
    off_flag = true;
}



void PlusMinus::turnOn(){
    off_flag = false;
    updateVal();
}


int PlusMinus::readFileValue(int fd) {
    lseek(fd, 0, SEEK_SET);
    char buffer[32] = {0};
    size_t br = read(fd, buffer, 32);
    return atoi(buffer);
}

void PlusMinus::incrTimeOut(){
    timeOutDuration+=timeIncr;
    tlbl->setText(QString::number(timeOutDuration) + "s");
    if(timeOutDuration>0){
        numTimers++;
        QTimer::singleShot(timeOutDuration*1000,this,SLOT(OnTimeOut()));
    }
}
void PlusMinus::decrTimeOut(){
    if(timeOutDuration-timeIncr>=0) timeOutDuration-=timeIncr;
    if(timeOutDuration>0){
        tlbl->setText(QString::number(timeOutDuration) + "s");
    }else{
        tlbl->setText("None");
    }
}


//Credit to https://zetcode.com/gui/qt5/firstprograms/ for the initial button plus minus code
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
    auto *plsBtn = new QPushButton("Increase Brightness", this);
    auto *minBtn = new QPushButton("Decrease Brightness", this);
    auto *closeBtn = new QPushButton("Exit", this);
    auto *off = new QPushButton("Off", this); 
    auto *on = new QPushButton("On", this);
    lbl = new QLabel("Brightness: " + QString::number(val) + "%", this);
    debug = new QLabel("");
    bar = new QProgressBar(this);
    bar->setValue(val);
    bar->setOrientation(Qt::Vertical);
    auto *grid = new QGridLayout(this);
    grid->addWidget(plsBtn, 0, 0, 2, 1);
    grid->addWidget(minBtn, 1, 0, 2, 1);
    grid->addWidget(closeBtn, 2, 0, 2, 1 );
    grid->addWidget(lbl, 0, 1,Qt::AlignCenter);
    grid->addWidget(bar,1,1,3,1,Qt::AlignCenter);
    grid->addWidget(debug, 1,1,1,1);
    grid->addWidget(off, 2.5, 0, 2 ,1);
    grid->addWidget(on, 2.5, 1, 2 ,1);
    //bar->setGeometry(300,50,75,200);
//    plsBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//    minBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    plsBtn->setMinimumSize(100,50);
    minBtn->setMinimumSize(100,50);
    closeBtn->setMinimumSize(50,25);
    bar->setMinimumSize(75,200);
    debug->setMinimumSize(50,50);
    off->setMinimumSize(50, 25);
    on->setMinimumSize(50, 25);



//    grid->setColumnMinimumWidth(1,70);
//    grid->setColumnMinimumWidth(2,70);

    setLayout(grid);

    connect(plsBtn, &QPushButton::clicked, this, &PlusMinus::OnPlus);
    connect(minBtn, &QPushButton::clicked, this, &PlusMinus::OnMinus);
    connect(closeBtn, &QPushButton::clicked, this, &PlusMinus::exit);
    connect(off, &QPushButton::clicked, this, &PlusMinus::turnOff);
    connect(on, &QPushButton::clicked, this, &PlusMinus::turnOn);
    
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
            int value = readFileValue(fd);
            debug->setText("Value is now\n" + QString::number(value) + " checked: " + QString::number(count));
            if(value == 1 && !off_flag) updateVal(); 

        }
    }
    // Q_UNUSED(str); 
    // debug->setText("Interrupt callback");
    count ++;
    ::close(fd);
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

void PlusMinus::exit(){
    QProcess::startDetached("/root/exit.sh", QStringList{});
    QApplication::exit();
}


void PlusMinus::turnOff(){
    lbl->setText("Brightness: " + QString::number(0) + "%");
    bar->setValue(0);
    QProcess::startDetached("/root/change.sh", QStringList {QString::number(0)});
    off_flag = true;
    QTimer::singleShot(timeOutDuration*1000,this,SLOT(OnTimeOut()));
}


void PlusMinus::offTimer(){
    off_flag = true;
}

void PlusMinus::turnOn(){
    updateVal();
}


int PlusMinus::readFileValue(int fd) {
    lseek(fd, 0, SEEK_SET);
    char buffer[32] = {0};
    size_t br = read(fd, buffer, 32);
    return atoi(buffer);
}


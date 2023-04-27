
#ifndef PLUSMINUS_H
#define PLUSMINUS_H

#include <QWidget>
#include <QApplication>
#include <QPushButton>
#include <QProgressBar>
#include <QLabel>
#include <iostream>
#include <fstream>
#include <string>
#include <QProcess>
#include <QTimer>
#include <QFileSystemWatcher>
#include <unistd.h>
#include <poll.h>
#include <fcntl.h>


class PlusMinus : public QWidget {

    Q_OBJECT

public:
    // QFileSystemWatcher *watcher;
    PlusMinus(QWidget *parent = nullptr);
    int readFileValue(int fd);

// public slots: 
    
private slots:
    void OnPlus();
    void OnMinus();
    void OnTimeOut();
    void exit();
    void switchChanged();
    void turnOn();
    void turnOff();
    void incrTimeOut();
    void decrTimeOut();


private:
    QLabel *lbl;
    QLabel *tlbl;
    QProgressBar *bar;
    QTimer *timer;
    //Brightness Value, 0-100
    int val;
    //Number of Running Timeout Timers, Lights shut off when = 0
    int numTimers;
    //Changes brightness increment of push buttons
    const int brightIncr = 5;
    //Changes time increment of push buttons (in seconds)
    const int timeIncr = 5;
    //Changes Default Timeout Duration (in seconds), after which lights shut off
    int timeOutDuration = 15;
    void updateVal();
    int count;
    bool off_flag;
};
#endif // PLUSMINUS_H

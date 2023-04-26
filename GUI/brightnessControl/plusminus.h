
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


class PlusMinus : public QWidget {

    Q_OBJECT

public:
    PlusMinus(QWidget *parent = nullptr);

private slots:
    void OnPlus();
    void OnMinus();
    void OnTimeOut();

private:
    QLabel *lbl;
    QProgressBar *bar;
    //Brightness Value, 0-100
    int val;
    //Number of Running Timeout Timers, Lights shut off when = 0
    int numTimers;
    //Changes brightness increment of push buttons
    const int brightIncr = 5;
    //Changes Timeout Duration (in seconds), after which lights shut off
    const int timeOutDuration = 15;
    void updateVal();
};
#endif // PLUSMINUS_H

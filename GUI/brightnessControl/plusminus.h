
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


class PlusMinus : public QWidget {

    Q_OBJECT

public:
    QFileSystemWatcher watcher;
    PlusMinus(QWidget *parent = nullptr);

public slots: 
    void   switchChanged(QString);
    
private slots:
    void OnPlus();
    void OnMinus();

private:
    QLabel *lbl;
    QProgressBar *bar;
    int val;
    //Changes brightness increment of push buttons
    const int brightIncr = 5;
    //Changes filename that brightness values are written to
    const char* fileName = "test.txt";
    void updateVal();
};
#endif // PLUSMINUS_H

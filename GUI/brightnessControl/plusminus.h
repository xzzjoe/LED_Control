
#ifndef PLUSMINUS_H
#define PLUSMINUS_H

#include <QWidget>
#include <QApplication>
#include <QPushButton>
#include <QProgressBar>
#include <QLabel>


class PlusMinus : public QWidget {

    Q_OBJECT

public:
    PlusMinus(QWidget *parent = nullptr);

private slots:
    void OnPlus();
    void OnMinus();

private:
    QLabel *lbl;
    QProgressBar *bar;
    int val;
    const int brightIncr = 5;
};
#endif // PLUSMINUS_H

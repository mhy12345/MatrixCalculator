#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QWidget>
#include <QToolButton>
#include <QtWidgets>
#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include "button.h"
#include "matrix.h"
using namespace std;

class Calculator : public QWidget
{
    Q_OBJECT

public:
    explicit Calculator(QWidget *parent = 0);
    void writeLog(QString log,QString color="black");
    ~Calculator();
private slots:
    void numbersKeyDown();
    void haha();
    void matrixListItemEdit(QListWidgetItem*);
signals:

private:
    map<QString,Matrix> data;
    map<QString,Fraction> vdata;
    QTextBrowser* display;
    QTextBrowser* matrixTable;
    QListWidget* matrixList;
    int inputMode;
    QString currentLine;
    QString matrixName;
private:
    Button* createButton(QString text,const char* event);
    void keyPressEvent(QKeyEvent  *event);
    void finishLine();
    bool eventFilter(QObject *ob, QEvent *e);
    void matrixListRefresh();
};

#endif // CALCULATOR_H

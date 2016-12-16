#include "redactor.h"
#include <QGraphicsItem>

#include <QDebug>

Redactor::Redactor(QWidget *parent) : QWidget(parent)
{
    strMapW = new QTextEdit(this);
    strMapW->setGeometry(1650,100,200,50);
    strMapW->show();

    view = new QGraphicsView(this);
    view->setGeometry(10,10,1600,860);
    view->show();
    redactScene = new QGraphicsScene(this);

    for (int i=0; i<100; i++) {
        redactScene->addLine(0 + 20*i,0,0 + 20*i,860);
    }

    for (int i=0; i<100; i++) {
        redactScene->addLine(0,0 + 20*i,1600,0 + 20*i);
    }

    view->setScene(redactScene);
}


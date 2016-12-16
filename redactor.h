#ifndef REDACTOR_H
#define REDACTOR_H

#include <QWidget>
#include <QLabel>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTextEdit>

class Redactor : public QWidget
{
    Q_OBJECT
public:
    explicit Redactor(QWidget *parent = 0);

    enum MapUnit {
        Floor,
        Wall,
        WoodWall,
        Window,
        Grass,
        WarningGrass,
        Stand,
        Door,
        StoneWall
    };

signals:

public slots:

private:
    QLabel labelWall;
    QLabel labelDoor;
    QLabel labelWWindow;
    QLabel labelStand;
    QGraphicsScene *redactScene;
    QGraphicsView *view;
    int mapW,mapH;
    QTextEdit *strMapW;
    QTextEdit *strMapH;
};

#endif // REDACTOR_H

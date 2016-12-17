#ifndef REDACTOR_H
#define REDACTOR_H

#include <QWidget>
#include <QLabel>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLineEdit>
#include <vector>

class Redactor : public QWidget
{
    Q_OBJECT
public:
    explicit Redactor(QWidget *parent = 0);

    enum MapUnit {
        Floor,
        Wall,
        EWindow,
        WWindow,
        SStand,
        Stand,
        InfStand,
        Door,
        WhiteWoodDoor
    };
private:
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);

signals:

public slots:

private:
    std::vector <std::vector <int> > mapMatrix;
    int state;
    QLabel *labelWall;
    QLabel *labelDoor;
    QLabel *labelWWindow;
    QLabel *labelStand;
    QGraphicsScene *redactScene;
    QGraphicsView *view;
    int mapW,mapH;
    QLineEdit *strMapW;
    QLineEdit *strMapH;
    int scaleSq;
};

#endif // REDACTOR_H

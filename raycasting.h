#ifndef RAYCASTING
#define RAYCASTING

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

#include <QLabel>

#include <math.h>

#include <QDebug>

#include "readlevel.h"

double sensitivity = 0.02;
const double eps = 0.00001;
class Raycasting: public QWidget
{
public:
    Raycasting(QWidget *parent = 0);

    void updatePlayer(bool rotate);

    void showFps();

    void render();

protected:

    void timerEvent(QTimerEvent*);

    void paintEvent(QPaintEvent *event);

    void keyPressEvent(QKeyEvent *event);

    void keyReleaseEvent(QKeyEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

private:
    QTime watch;
    QBasicTimer ticker;
    QImage buffer;
    qreal angle;
    QPointF playerPos;
    qreal angleDelta;
    qreal moveDelta;
    qreal rotateDelta;
    QImage textureImg;
    int textureCount;
    bool ROT;
    const QPoint screenCentre = QApplication::desktop()->screenGeometry().center();

};



#endif // RAYCASTING


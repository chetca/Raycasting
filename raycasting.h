#ifndef RAYCASTING
#define RAYCASTING

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

#include <QLabel>

#include <cmath>

class Raycasting: public QWidget
{
    Q_OBJECT
public:
    Raycasting(QWidget *parent = 0);
    ~Raycasting();
    void updatePlayer();
    void showFps();
    void render();    
    void setPersToCentre();
    void loadTexture(int bufh, QRgb *start, qreal texofs, qreal hitdist, QRgb *finish, int texture, bool dark, const QRgb *texsrc, int bufw, int ray, QRgb stride);

protected:
    void timerEvent(QTimerEvent*);
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private:
    QTime watch;
    QBasicTimer ticker;
    QImage buffer;
    qreal angle;
    QPointF playerPos;
    qreal angleDelta;
    qreal moveDelta;
    qreal moveDelta2;
    qreal rotateDelta;
    QImage textureImg;
    QLabel *FPS;
    QLabel *PERS;
    int textureCount;
    const QPoint screenCentre = QApplication::desktop()->screenGeometry().center();

    double sensitivity = 0.02;
    const double eps = 0.00001;
    const double PlayerSpeed = 2.5;
    const double runSpeed = 10;
};



#endif // RAYCASTING


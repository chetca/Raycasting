#include "redactor.h"
#include <QGraphicsItem>

#include <QDebug>
#include <QPicture>
#include <QPixmap>
#include <QTransform>
#include <QKeyEvent>
#include <QRect>
#include <QGraphicsItem>

#include <QDebug>

Redactor::Redactor(QWidget *parent) : QWidget(parent)
{
    state = -1;
    QLabel *labelMapW = new QLabel(this);
    labelMapW->setGeometry(1680,80,150,30);
    labelMapW->setText("Width");

    QCursor cur;
    setCursor(cur);

    QLabel *LabelMapH = new QLabel(this);
    LabelMapH->setGeometry(1680,120,150,30);
    LabelMapH->setText("Height");

    strMapW = new QLineEdit(this);
    strMapW->setGeometry(1730,80,150,30);
    strMapW->show();

    strMapH = new QLineEdit(this);
    strMapH->setGeometry(1730,120,150,30);
    strMapH->show();

    qDebug() << strMapW->text();

    scaleSq = 20;

    mapW = 78;
    mapH = 43;

    mapMatrix.resize(mapH, std::vector <int> (mapW));

    view = new QGraphicsView(this);
    view->setGeometry(10,10,1600,860);
    view->show();
    redactScene = new QGraphicsScene(this);

    for (int i=0; i<mapW; i++) {
        redactScene->addLine(0 + scaleSq*i,0,0 + scaleSq*i,(mapH-1)*scaleSq);
    }

    for (int i=0; i<mapH; i++) {
        redactScene->addLine(0,0 + scaleSq*i,(mapW-1)*scaleSq,0 + scaleSq*i);
    }

    redactScene->setBackgroundBrush(QBrush(QColor(200, 200, 200)));

    view->setScene(redactScene);

    labelWall = new QLabel(this);
    labelWall->setGeometry(1680,180,64,64);

    labelDoor = new QLabel(this);
    labelDoor->setGeometry(1680,260,64,64);

    labelStand = new QLabel(this);
    labelStand->setGeometry(1680,340,64,64);

    labelWWindow = new QLabel(this);
    labelWWindow->setGeometry(1680,420,64,64);

    QPixmap *pixmapWall = new QPixmap(":/textures.png");
    QTransform tr = QTransform().rotate(90);
    QPixmap *trWall = new QPixmap(pixmapWall->transformed(tr));
    labelWall->setPixmap(trWall->copy(512-64,0,64,64));
    labelDoor->setPixmap(trWall->copy(512-64*7,0,64,64));
    labelStand->setPixmap(trWall->copy(512-64*6,0,64,64));
    labelWWindow->setPixmap(trWall->copy(512-64*3,0,64,64));
    delete pixmapWall;
}

void Redactor::keyPressEvent(QKeyEvent *event)
{
    event->accept();
    if (event->key() == Qt::Key_Escape) {
        this->close();
    }
}

void Redactor::mousePressEvent(QMouseEvent *event)
{
    if (event->button()==Qt::LeftButton) {
        QRect LW = labelWall->geometry();
        QRect LD = labelDoor->geometry();
        QRect LS = labelStand->geometry();
        QRect LWW= labelWWindow->geometry();

        QPixmap curPix = cursor().pixmap();
        if (LW .contains(QRect(event->pos(),event->pos()))) {state = Wall;   curPix = QPixmap(*labelWall->pixmap()   );}
        if (LD .contains(QRect(event->pos(),event->pos()))) {state = Door;   curPix = QPixmap(*labelDoor->pixmap()   );}
        if (LS .contains(QRect(event->pos(),event->pos()))) {state = Stand;  curPix = QPixmap(*labelStand->pixmap()  );}
        if (LWW.contains(QRect(event->pos(),event->pos()))){state = WWindow; curPix = QPixmap(*labelWWindow->pixmap());}
        setCursor(QCursor(curPix.scaled(30,30)));

    }
    else if (event->button() == Qt::RightButton) {
        state = -1;
    }

    if (state!=-1) {
        QPixmap setTexture = QPixmap(cursor().pixmap());
        QPoint mapP = (event->pos() - view->pos() - QPoint(30,10))/scaleSq;
        if (mapP.x() > 0 && mapP.y()>0 && mapP.x() <= mapW && mapP.y() <= mapH) {
            mapMatrix[mapP.y()][mapP.x()] = state;
            QGraphicsItem *textureRed;// = new QGraphicsItem;
            textureRed->setPos(30 + mapP.x()*scaleSq,10+mapP.y()*scaleSq);
            textureRed->setData(1,QVariant(setTexture.scaled(20,20)));
            redactScene->addItem(textureRed);
        }
    }

    qDebug() << "setted state " << state;
}


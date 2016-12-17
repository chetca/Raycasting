#include "raycasting.h"

#include "readlevel.h"

#include <QDebug>

Raycasting::Raycasting(QWidget *parent) :
    QWidget(parent),
    angle(1.5),
    playerPos(47, 7),
    angleDelta(0),
    moveDelta(0)
{
    QCursor curs = cursor();
    curs.setShape(Qt::BlankCursor);
    setCursor(curs);

    ::readlevel();

    textureImg.load(":/textures.png");
    textureImg = textureImg.convertToFormat(QImage::Format_ARGB32);
    Q_ASSERT(textureImg.width() == TEXTURE_SIZE * 2);
    Q_ASSERT(textureImg.bytesPerLine() == 4 * TEXTURE_SIZE * 2);
    textureCount = textureImg.height() / TEXTURE_SIZE;

    watch.start();
    ticker.start(5, this);
    setAttribute(Qt::WA_OpaquePaintEvent, true);
    setMouseTracking(1);
    cursor().setPos(QApplication::desktop()->screenGeometry().center());
    cursor().bitmap();

    FPS = new QLabel(this);
    FPS->setGeometry(100,100,500,50);
    FPS->show();//оно жжот

    int SCREENWIDTH = QApplication::desktop()->screenGeometry().width();
    int SCREENHEIGHT = QApplication::desktop()->screenGeometry().height();
//    PERS = new QLabel(this);
//    setPersToCentre();
//    PERS->setPixmap((QPixmap(":/za.png")));
//    PERS->show();

    QPixmap *hands = new QPixmap(":/za.png");
    LeftHand = new QLabel(this);
    LeftHand->setPixmap(hands->copy(0,0,980,600).scaled(SCREENWIDTH/4,SCREENHEIGHT/3));
    LeftHand->setGeometry(0,SCREENHEIGHT-SCREENHEIGHT/3,950,SCREENHEIGHT/3);
    LeftHand->show();

    RightHand = new QLabel(this);
    RightHand->setPixmap(hands->copy(980,0,1900,600).scaled(SCREENWIDTH/3,SCREENHEIGHT/3));
    RightHand->setGeometry(SCREENWIDTH-SCREENWIDTH/3,SCREENHEIGHT-SCREENHEIGHT/3,SCREENWIDTH/3,SCREENHEIGHT/3);
    RightHand->show();
}

Raycasting::~Raycasting()
{

}

void Raycasting::updatePlayer() {
    int interval = qBound(20, watch.elapsed(), 250);
    watch.start();
    angle += angleDelta * interval / 1000;
    qreal step = moveDelta * interval / 1000;
    qreal step2= moveDelta2* interval / 1000;
    qreal dx = cos(angle) * step;
    qreal dy = sin(angle) * step;
    qreal dx2 =-sin(angle) * step2,
          dy2 = cos(angle) * step2;

    QPointF pos = playerPos + 3 * QPointF(dx, dy) + 3 * QPointF(dx2,dy2);
    int xi = static_cast<int>(pos.x());
    int yi = static_cast<int>(pos.y());
    if (world_map[yi][xi] == 0)
        playerPos = playerPos + QPointF(dx, dy) + QPointF(dx2,dy2);
}

void Raycasting::showFps() {
    static QTime frameTick;
    static int totalFrame = 0;
    if (!(totalFrame & 31)) {
        int elapsed = frameTick.elapsed();
        frameTick.start();
        int fps = 32 * 1000 / (1 + elapsed);
        FPS->setText(QString("(%1 FPS)").arg(fps));
        FPS->setFont(QFont("Times", 40, QFont::Bold));
        //FPS.show();
        //setWindowTitle(QString("Raycasting (%1 FPS)").arg(fps));
    }

    totalFrame++;
}

void Raycasting::loadTexture(int bufh, QRgb *start, qreal texofs, qreal hitdist, QRgb *finish, int texture, bool dark, const QRgb *texsrc, int bufw, int ray, QRgb stride)
{
    int col = static_cast<int>(texofs * TEXTURE_SIZE);
    col = qBound(0, col, TEXTURE_SIZE - 1);
    texture = (texture - 1) % textureCount;
    const QRgb *tex = texsrc + TEXTURE_BLOCK * texture * 2 +
            (TEXTURE_SIZE * 2 * col);
    if (dark)
        tex += TEXTURE_SIZE;

    // запуск из центра текстуры (по горизонтали)
    int h = static_cast<int>(bufw / hitdist / 2);
    int dy = (TEXTURE_SIZE << 12) / h;
    int p1 = ((TEXTURE_SIZE / 2) << 12) - dy;
    int p2 = p1 + dy;

    // запуск с центра экрана (по вертикали)
    // y1 будет идти вверх (уменьшение), у2 будет идти вниз (увеличение)
    int y1 = bufh / 2;
    int y2 = y1 + 1;
    QRgb *pixel1 = start + y1 * stride + ray;
    QRgb *pixel2 = pixel1 + stride;

    // карта текстуры серого цвета
    while (y1 >= 0 && y2 < bufh && p1 >= 0) {
        *pixel1 = tex[p1 >> 12];
        *pixel2 = tex[p2 >> 12];
        p1 -= dy;
        p2 += dy;
        --y1;
        ++y2;
        pixel1 -= stride;
        pixel2 += stride;
    }

    // потолок и пол
    for (; pixel1 > start; pixel1 -= stride)
        *pixel1 = qRgb(227, 227, 255); //известь
    for (; pixel2 < finish; pixel2 += stride)
        *pixel2 = qRgb(133, 133, 133);
}

void Raycasting::render() {

    // настройка поверхности экрана
    if (buffer.size() != size())
        buffer = QImage(size(), QImage::Format_ARGB32);
    int bufw = buffer.width();
    int bufh = buffer.height();

    // мы специально читерим ято бы не выбросило
    const uchar *ptr = buffer.bits();
    QRgb *start = (QRgb*)(ptr);
    QRgb stride = buffer.bytesPerLine() / 4;
    QRgb *finish = start + stride * bufh;

    // подготовка указателя текстурирования
    const uchar *src = textureImg.bits();
    const QRgb *texsrc = reinterpret_cast<const QRgb*>(src);

    // здесь бросаем все лучи
    qreal sina = sin(angle);
    qreal cosa = cos(angle);
    qreal u = cosa - sina;
    qreal v = sina + cosa;
    qreal du = 2 * sina / bufw;
    qreal dv = -2 * cosa / bufw;

    for (int ray = 0; ray < bufw; ++ray, u += du, v += dv) {
        // каждый раз, когда луч проходит u единиц в направлении х,
        // он проходит v единиц в направлении y
        qreal uu = (u < 0) ? -u : u;
        qreal vv = (v < 0) ? -v : v;
        qreal duu = 1 / uu;
        qreal dvv = 1 / vv;
        int stepx = (u < 0) ? -1 : 1;
        int stepy = (v < 0) ? -1 : 1;

        // ячейка карты которую нам нужно проверить
        qreal px = playerPos.x();
        qreal py = playerPos.y();
        int mapx = static_cast<int>(px);
        int mapy = static_cast<int>(py);

        // позиция и текстура для удара
        int texture = 0;
        qreal hitdist = 0.1;
        qreal texofs = 0;
        bool dark = false;

        // первый удар луча для постоянных x и y
        qreal distx = (u > 0) ? (mapx + 1 - px) * duu : (px - mapx) * duu;
        qreal disty = (v > 0) ? (mapy + 1 - py) * dvv : (py - mapy) * dvv;

        // цикл до тех пор пока луч не встретит что-либо
        while (texture <= 0) {
            if (distx > disty) {
                // кратчайшая дистанция до удара на константной линии y
                hitdist = disty;
                disty += dvv;
                mapy += stepy;
                texture = world_map[mapy][mapx];
                if (texture > 0) {
                    dark = true;
                    if (stepy > 0) {
                        qreal ofs = px + u * (mapy - py) / v;
                        texofs = ofs - floor(ofs);
                    } else {
                        qreal ofs = px + u * (mapy + 1 - py) / v;
                        texofs = ofs - floor(ofs);
                    }
                }
            } else {
                // кратчайшая дистанция до удара на константной линии x
                hitdist = distx;
                distx += duu;
                mapx += stepx;
                texture = world_map[mapy][mapx];
                if (texture > 0) {
                    if (stepx > 0) {
                        qreal ofs = py + v * (mapx - px) / u;
                        texofs = ofs - floor(ofs);
                    } else {
                        qreal ofs = py + v * (mapx + 1 - px) / u;
                        texofs = ceil(ofs) - ofs;
                    }
                }
            }
        }

        // получаем текстуру с учётом изображения на ней
        // различаем два вида одной текстуры: "normal" и "dark"
        loadTexture(bufh, start, texofs, hitdist, finish, texture, dark, texsrc, bufw, ray, stride);
    }

    update();
}

void Raycasting::timerEvent(QTimerEvent *) {
    angleDelta = -(QCursor::pos().x()-screenCentre.x())*sensitivity;
    QCursor::setPos(screenCentre);

    updatePlayer();
    render();
    showFps();
}

void Raycasting::paintEvent(QPaintEvent *event) {
    QPainter p(this);
    p.setCompositionMode(QPainter::CompositionMode_Source);
    p.drawImage(event->rect(), buffer, event->rect());
}

void Raycasting::keyPressEvent(QKeyEvent *event) {
    event->accept();
    if (event->key() == Qt::Key_Escape) {
        this->~Raycasting();
    }

    if (event->key() == Qt::Key_A){ //шаг влево
        moveDelta2 = PlayerSpeed;
    }
    if (event->key() == Qt::Key_D){ //шаг вправо
        moveDelta2 = -PlayerSpeed;
    }
    if (event->key() == Qt::Key_W && QGuiApplication::keyboardModifiers()==Qt::NoModifier){ //вперёд
        moveDelta = PlayerSpeed;
    }
    if (event->key() == Qt::Key_S && QGuiApplication::keyboardModifiers()==Qt::NoModifier){ //назад
        moveDelta = -PlayerSpeed;
    }

    if (event->key() == Qt::Key_W && QGuiApplication::keyboardModifiers()==Qt::ShiftModifier) {
        moveDelta = runSpeed;
    }

}

void Raycasting::keyReleaseEvent(QKeyEvent *event) {
    event->accept();

    if (QGuiApplication::keyboardModifiers()==Qt::ShiftModifier) {
        moveDelta = 0;
    }
    if (event->key() == Qt::Key_A)
        moveDelta2 = 0;
    if (event->key() == Qt::Key_D)
        moveDelta2 = 0;
    if (event->key() == Qt::Key_W)
        moveDelta = 0;
    if (event->key() == Qt::Key_S)
        moveDelta = 0;
    if (event->key() == Qt::Key_W && QGuiApplication::keyboardModifiers()==Qt::ShiftModifier) {
        moveDelta = 0;
    }


}

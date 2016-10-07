#include <QtCore>
#include <QtGui>
#include <QtWidgets>

#include <math.h>

#include "readlevel.h"

class Raycasting: public QWidget
{
public:
    Raycasting(QWidget *parent = 0)
            : QWidget(parent)
            , angle(0.5)
            , playerPos(1.5, 1.5)
            , angleDelta(0)
            , moveDelta(0)
    {
        ::readlevel();
        // http://www.areyep.com/RIPandMCS-TextureLibrary.html
        textureImg.load(":/textures.png");
        textureImg = textureImg.convertToFormat(QImage::Format_ARGB32);
        Q_ASSERT(textureImg.width() == TEXTURE_SIZE * 2);
        Q_ASSERT(textureImg.bytesPerLine() == 4 * TEXTURE_SIZE * 2);
        textureCount = textureImg.height() / TEXTURE_SIZE;

        watch.start();
        ticker.start(25, this);
        setAttribute(Qt::WA_OpaquePaintEvent, true);
    }

    void updatePlayer() {
        int interval = qBound(20, watch.elapsed(), 250);
        watch.start();
        angle += angleDelta * interval / 1000;
        qreal step = moveDelta * interval / 1000;
        qreal dx = cos(angle) * step;
        qreal dy = sin(angle) * step;
        QPointF pos = playerPos + 3 * QPointF(dx, dy);
        int xi = static_cast<int>(pos.x());
        int yi = static_cast<int>(pos.y());
        if (world_map[yi][xi] == 0)
            playerPos = playerPos + QPointF(dx, dy);
    }

    void showFps() {
        static QTime frameTick;
        static int totalFrame = 0;
        if (!(totalFrame & 31)) {
            int elapsed = frameTick.elapsed();
            frameTick.start();
            int fps = 32 * 1000 / (1 + elapsed);
            setWindowTitle(QString("Raycasting (%1 FPS)").arg(fps));
        }
        totalFrame++;
    }

    void render() {

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
                *pixel1 = qRgb(0, 0, 0);
            for (; pixel2 < finish; pixel2 += stride)
                *pixel2 = qRgb(96, 96, 96);
        }

        update();
    }

protected:

    void timerEvent(QTimerEvent*) {
        updatePlayer();
        render();
        showFps();
    }

    void paintEvent(QPaintEvent *event) {
        QPainter p(this);
        p.setCompositionMode(QPainter::CompositionMode_Source);
        p.drawImage(event->rect(), buffer, event->rect());
    }

    void keyPressEvent(QKeyEvent *event) {
        event->accept();
        if (event->key() == Qt::Key_Left)
            angleDelta = 1.3 * M_PI;
        if (event->key() == Qt::Key_Right)
            angleDelta = -1.3 * M_PI;
        if (event->key() == Qt::Key_Up)
            moveDelta = 2.5;
        if (event->key() == Qt::Key_Down)
            moveDelta = -2.5;
        if (event->key() == Qt::Key_Escape)
            this->close();
    }

    void keyReleaseEvent(QKeyEvent *event) {
        event->accept();
        if (event->key() == Qt::Key_Left)
            angleDelta = (angleDelta > 0) ? 0 : angleDelta;
        if (event->key() == Qt::Key_Right)
            angleDelta = (angleDelta < 0) ? 0 : angleDelta;
        if (event->key() == Qt::Key_Up)
            moveDelta = (moveDelta > 0) ? 0 : moveDelta;
        if (event->key() == Qt::Key_Down)
            moveDelta = (moveDelta < 0) ? 0 : moveDelta;
    }

private:
    QTime watch;
    QBasicTimer ticker;
    QImage buffer;
    qreal angle;
    QPointF playerPos;
    qreal angleDelta;
    qreal moveDelta;
    QImage textureImg;
    int textureCount;
};


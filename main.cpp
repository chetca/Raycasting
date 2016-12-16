#include <QtCore>
#include <QtGui>
#include <QtWidgets>

#include <cmath>

#include "raycasting.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    Raycasting w;

    w.setWindowTitle("Raycasting");
    w.showFullScreen();

    return app.exec();
}

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

#include <math.h>

#include "readlevel.h"
#include "raycasting.cpp"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    Raycasting w;
    w.setWindowTitle("Raycasting");

    w.showFullScreen();

    w.show();


    return app.exec();
}

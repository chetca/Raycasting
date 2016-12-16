#include <QtCore>
#include <QtGui>
#include <QtWidgets>

#include <cmath>

#include "raycasting.h"
#include "menu.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    Menu w;

    w.show();

    return app.exec();
}

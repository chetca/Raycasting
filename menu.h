#ifndef MENU_H
#define MENU_H

#include <QWidget>
#include "raycasting.h"
#include "redactor.h"

namespace Ui {
class Menu;
}

class Menu : public QWidget
{
    Q_OBJECT

public:
    explicit Menu(QWidget *parent = 0);
    ~Menu();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Menu *ui;
};

#endif // MENU_H

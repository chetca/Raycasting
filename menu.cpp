#include "menu.h"
#include "ui_menu.h"

Menu::Menu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Menu)
{
    ui->setupUi(this);
}

Menu::~Menu()
{
    delete ui;
}

void Menu::on_pushButton_clicked()
{
    Raycasting *w = new Raycasting;
    w->setWindowTitle("Raycasting");
    w->showFullScreen();
    w->setPersToCentre();
}

void Menu::on_pushButton_2_clicked()
{
    this->hide();
    Redactor *redactor = new Redactor;
    redactor->showFullScreen();
}

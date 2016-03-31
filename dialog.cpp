#include "dialog.hpp"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog) {
    ui->setupUi(this);
    QGraphicsScene *scene = new QGraphicsScene();
    ui->graphicsView->setScene(scene);
}

Dialog::~Dialog() {
    delete ui;
}



void Dialog::on_pushButton_clicked() {          //what happens when we click the "Kolejna tura" button

}

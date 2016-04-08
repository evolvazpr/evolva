#include "dialog.hpp"
#include "ui_dialog.h"

static const uint ID_KEY = 0;
static const uint DX_KEY = 1;
static const uint DY_KEY = 2;

static const qreal INCREMENT_PER_TICK = 1;

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog), width_(25), height_(25) {
    ui->setupUi(this);
    scene = new QGraphicsScene;
    ui->graphicsView->setScene(scene);
    scene->setSceneRect(ui->graphicsView->rect());
    ui->graphicsView->show();

    QObject::connect(&timer, SIGNAL(timeout()), this, SLOT(test()));
}

Dialog::~Dialog() {
    delete ui;
}

void Dialog::on_pushButton_clicked() {          //what happens when we click the "Kolejna tura" button

}

qreal Dialog::calculate_x(const uint x) {
    return floor((qreal)x / (qreal)width_ * scene->width());
}

qreal Dialog::calculate_y(const uint y) {
    return floor((qreal)y / (qreal)height_ * scene->height());
}

qreal Dialog::calculate_radius() {
    return  floor(scene->height()/height_);
}

void Dialog::createObject(const uint id, const uint x, const uint y) {
    qreal x_pos = calculate_x(x);
    qreal y_pos = calculate_y(y);
    qreal radius =  calculate_radius();
    QGraphicsEllipseItem *object = new QGraphicsEllipseItem();
    object->setData(ID_KEY, QVariant(id));
    object->setRect(x_pos, y_pos, radius, radius);
    scene->addItem(object);
}

QGraphicsItem* Dialog::searchObject(const uint id) {
    QList<QGraphicsItem *> obj_list = scene->items();
    for (auto it : obj_list) {
        if(it->data(ID_KEY) == id)
            return it;
    }
    return 0;
}

void Dialog::test() {
    QList<QGraphicsItem *> obj_list = scene->items();
    qreal dx;
    qreal dy;
    qreal x;
    qreal y;
    for (auto it : obj_list) {
        dx = floor(it->data(DX_KEY).toDouble());
        dy = floor(it->data(DY_KEY).toDouble());
        if ((!dx) && (!dy))
            continue;
        if (dx > 0) {
            dx -= INCREMENT_PER_TICK;
            x = it->x() + INCREMENT_PER_TICK;
        } else {
            dx += INCREMENT_PER_TICK;
            x = it->x() - INCREMENT_PER_TICK;
        }
        if (dy > 0) {
            dy -= INCREMENT_PER_TICK;
            y = it->y() + INCREMENT_PER_TICK;
        } else {
            dy += INCREMENT_PER_TICK;
            y = it->y() - INCREMENT_PER_TICK;
        }
        it->setPos(x, y);
        it->setData(DX_KEY, QVariant(dx));
        it->setData(DY_KEY, QVariant(dy));
    }
}

 void Dialog::moveObject(const uint id, const uint x, const uint y) {
        QGraphicsItem *object = searchObject(id);
        if (!object) return; //TODO: throw exception
        object->setData(DX_KEY, QVariant(object->data(DX_KEY).toDouble() + calculate_x(x)));
        object->setData(DY_KEY, QVariant(object->data(DY_KEY).toDouble() + calculate_y(y)));
        if(!timer.isActive())
            timer.start(1000/33);
 }

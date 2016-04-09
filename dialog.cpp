#include "dialog.hpp"
#include "ui_dialog.h"

static const uint ID_KEY = 0;
static const uint DX_KEY = 1;
static const uint DY_KEY = 2;

static const qreal INCREMENT_PER_TICK = 1;

static const qreal ANIMATION_CLOCK = 1000/33;

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

int Dialog::calculate_x(const uint x) {
    return floor((qreal)x / (qreal)width_ * scene->width());
}

int Dialog::calculate_y(const uint y) {
    return floor((qreal)y / (qreal)height_ * scene->height());
}

int Dialog::calculate_radius() {
    return  floor(scene->height()/height_);
}

void Dialog::createObject(const uint id, const uint x, const uint y) {
    int x_pos = calculate_x(x);
    int y_pos = calculate_y(y);
    int radius =  calculate_radius();
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
    int dx;
    int dy;
    int x;
    int y;
    for (auto it : obj_list) {
        dx = floor(it->data(DX_KEY).toInt());
        dy = floor(it->data(DY_KEY).toInt());
        if ((!dx) && (!dy))
            continue;

        if (dx > 0) {
            dx -= INCREMENT_PER_TICK;
            x = it->x() + INCREMENT_PER_TICK;
        } else if (dx < 0) {
            dx += INCREMENT_PER_TICK;
            x = it->x() - INCREMENT_PER_TICK;
        } else {
            x = it->x();
        }

        if (dy > 0) {
            dy -= INCREMENT_PER_TICK;
            y = it->y() + INCREMENT_PER_TICK;
        } else if (dy < 0) {
            dy += INCREMENT_PER_TICK;
            y = it->y() - INCREMENT_PER_TICK;
        } else {
            y = it->y();
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
            timer.start(ANIMATION_CLOCK);
 }

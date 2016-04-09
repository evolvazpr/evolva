#include "dialog.hpp"
#include "ui_dialog.h"

static const uint ID_KEY = 0;
static const uint DX_KEY = 1;
static const uint DY_KEY = 2;

static const qreal INCREMENT_PER_TICK = 5;

static const qreal ANIMATION_CLOCK = 1000/33;

Dialog::Dialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::Dialog), width_(50), height_(50) {
	QRect rect;
	ui->setupUi(this);
	scene = new QGraphicsScene;
	ui->graphicsView->setScene(scene);

	rect = ui->graphicsView->rect();
	scene->setSceneRect(rect.x(), rect.y(), rect.height()-4, rect.height()-4);
	ui->graphicsView->show();
}

Dialog::~Dialog() {
	delete ui;
}

void Dialog::on_pushButton_clicked() {          //what happens when we click the "Kolejna tura" button

}

int Dialog::calculate_x(const uint x) {
	return (qreal)x / (qreal)width_ * scene->width();
}

int Dialog::calculate_y(const uint y) {
	return (qreal)y / (qreal)height_ * scene->height();
}

int Dialog::calculate_radius() {
	return  scene->height()/height_;
}

void Dialog::createObject(const uint id, const uint x, const uint y) {
	int x_pos = calculate_x(x);
	int y_pos = calculate_y(y);
	int radius =  calculate_radius();
	RoundObject *object = new RoundObject(x, y, radius, scene);
	object->setData(ID_KEY, QVariant(id));
	object->setRect(x_pos, y_pos, radius, radius);
	QObject::connect(&timer, SIGNAL(timeout()), object, SLOT(animate()));
}

QGraphicsItem* Dialog::searchObject(const uint id) {
	QList<QGraphicsItem *> obj_list = scene->items();
	for (auto it : obj_list) {
		if(it->data(ID_KEY) == id)
			return it;
	}
	return 0;
}

RoundObject::RoundObject(const uint x, const uint y, const uint radius, QGraphicsScene *scene) : QObject(),
	QGraphicsEllipseItem(x, y, radius, radius, 0, scene) {}

void RoundObject::animate() {
	int dx;
	int dy;
	int x_coord;
	int y_coord;

	/* Some calculations */
	dx = data(DX_KEY).toInt();
	dy = data(DY_KEY).toInt();
	if ((!dx) && (!dy))
		return;
	if (dx > 0) {
		if (dx >= INCREMENT_PER_TICK) {
			dx -= INCREMENT_PER_TICK;
			x_coord = x() + INCREMENT_PER_TICK;
		} else {
			x_coord = x() + dx;
			dx = 0;
		}
	} else if (dx < 0) {
		if (std::abs(dx) >= INCREMENT_PER_TICK) {
			dx += INCREMENT_PER_TICK;
			x_coord = x() - INCREMENT_PER_TICK;
		} else {
			x_coord = x() + dx;
			dx = 0;
		}
	} else {
		x_coord = x();
	}

	if (dy > 0) {
		if (dy >= INCREMENT_PER_TICK) {
			dy -= INCREMENT_PER_TICK;
			y_coord = y() + INCREMENT_PER_TICK;
		} else {
			y_coord = y() + dy;
			dy = 0;
		}
	} else if (dy < 0) {
		if (std::abs(dy) >= INCREMENT_PER_TICK) {
			dy += INCREMENT_PER_TICK;
			y_coord = y() - INCREMENT_PER_TICK;
		} else {
			y_coord = y() + dy;
			dy = 0;
		}
	} else {
		y_coord = y();
	}
	/* End of some calculations */

	setPos(x_coord, y_coord);
	setData(DX_KEY, QVariant(dx));
	setData(DY_KEY, QVariant(dy));
}

void Dialog::moveObject(const uint id, const int x, const int y) {
	QGraphicsItem *object = searchObject(id);
	if (!object) return; //TODO: throw exception
	object->setData(DX_KEY, QVariant(calculate_x(x)));
	object->setData(DY_KEY, QVariant(calculate_y(y)));
	if(!timer.isActive())
		timer.start(ANIMATION_CLOCK);
}

void Dialog::moveObjectTo(const uint id, const int x, const int y) {
	QGraphicsItem *object = searchObject(id);
	int x_old, y_old, dx, dy;
	if (!object) return; //TODO: throw exception
	x_old = object->x();
	y_old = object->y();
	dx = calculate_x(x) - x_old;
	dy = calculate_y(y) - y_old;
	object->setData(DX_KEY, QVariant(dx));
	object->setData(DY_KEY, QVariant(dy));
	if(!timer.isActive())
		timer.start(ANIMATION_CLOCK);
}

void Dialog::removeObject(const uint id) {
	RoundObject *object = dynamic_cast<RoundObject *>(searchObject(id));
	if (!object) return; //TODO: throw exception
	QObject::disconnect(&timer, SIGNAL(timeout()), object, SLOT(animate()));
	scene->removeItem(object);
}

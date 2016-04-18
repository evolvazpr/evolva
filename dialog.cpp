#include "dialog.hpp"
#include "ui_dialog.h"

/**
 * @brief INCREMENT_PER_TICK static global variable. Parameter to set speed of animation.
 */
static const qreal INCREMENT_PER_TICK = 5;

/**
 * @brief ANIMATION_CLOCK static global variable. Parameter to setup freqency of animation (FPS or whatever).
 */
static const qreal ANIMATION_CLOCK = 1000/33;

/**
 * @brief FIELD_SIZE temporary?
 */
static const uint FIELD_SIZE = 50;

/**
 * @brief Pixels per object
 */
static const uint PIXELS_PER_OBJECT = 30;

/**
 * @brief RoundObject constructor.
 * @param id - object's id.
 * @param x - object's graphical x coordinate (it is not field coordinate!).
 * To calculate graphical coordinates use Dialog::calculateX method.
 * @param y - object's graphical y coordinate (it is not field coordinate!).
 * To calculate graphical coordinates use Dialog::calculateY method.
 * @param radius - object's radius.
 * To calculate graphical radius use Dialog::calculateRadius method.
 * @param scene - pointer to scene.
 * @param timer - pointer to timer. It is needed, because on each move there is
 * established connection between timer signal timout() and RoundObject slot animate() (to animate or whatever).
 */
RoundObject::RoundObject(const uint id, const int x, const int y, const uint radius,
			QGraphicsScene *scene, QTimer *timer, QColor color) : 
			QObject(scene->parent()), QGraphicsEllipseItem(x, y, radius, radius),
		       	id_(id), timer_(timer) 
{
	setBrush(QBrush(color, Qt::SolidPattern));
	scene->addItem(this);	
}

/**
 * @brief RoundObject's deconstructor.
 */
RoundObject::~RoundObject() {
}


/**
 * @brief Object move method.
 *
 * It makes connection of timer's signals and object animate method.
 *
 * @param dx - how many steps in pixels to take into x direction.
 * @param dy - how many steps in pixels to take into y direction.
 */
void RoundObject::move(const int dx, const int dy) {
	dx_ = dx;
	dy_ = dy;
	QObject::connect(timer_, SIGNAL(timeout()), this, SLOT(animate()));
}

/**
 * @brief Method to obtain id number.
 * @return object's id number.
 */
uint RoundObject::id() {
	return id_;
}

/**
 * @brief Centerpiece of animation.
 *
 * This method is called when timer emits signal timeout().
 * It updates steps (dx, dy) that object must do, and updates
 * graphical coordinates of object. If there is no steps to perform,
 * method disconnects timer from this method.
 */
void RoundObject::animate() {
	int dx;
	int dy;
	int x_coord;
	int y_coord;

	/* Some calculations */
	dx = dx_;
	dy = dy_;
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
	dx_ = dx;
	dy_ = dy;
	if((!dx) && (!dy))
		QObject::disconnect(timer_, SIGNAL(timeout()), this, SLOT(animate()));
}

/**
 * @brief Constructor.
 * @param parent - parent for Qt API (no need to call delete thanks to it).
 */
Dialog::Dialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::Dialog), width_(FIELD_SIZE), height_(FIELD_SIZE) {
	QRect rect;
	ui->setupUi(this);
	scene = new QGraphicsScene();
	ui->graphicsView->setScene(scene);
	scene->setSceneRect(0, 0, FIELD_SIZE * PIXELS_PER_OBJECT, FIELD_SIZE * PIXELS_PER_OBJECT);
	ui->graphicsView->setSceneRect(scene->sceneRect());
	ui->graphicsView->setCacheMode(QGraphicsView::CacheNone);
	ui->graphicsView->setRenderHint(QPainter::Antialiasing);
	ui->graphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
	ui->graphicsView->show();
}

/**
 * @brief Deconstructor.
 */
Dialog::~Dialog() {
	delete ui;
}

void Dialog::on_pushButton_clicked() {          //what happens when we click the "Kolejna tura" button

}
/**
 * @brief Graphical x coordinate calculation.
 * @param x - field's x coordinate.
 * @return  - graphical x coordinate.
 */
int Dialog::calculateX(const int x) {
	return (qreal)x / (qreal)width_ * scene->width();
}

/**
 * @brief Graphical y coordinate calculation.
 * @param y - field's y coordinate.
 * @return  - graphical y coordinate.
 */
int Dialog::calculateY(const int y) {
	return (qreal)y / (qreal)height_ * scene->height();
}

/**
 * @brief Graphical radius coordinate calculation.
 * @return
 */
uint Dialog::calculateRadius() {
	return  scene->height()/height_;
}

/**
 * @brief RoundObject object creation.
 * @param id - object's id.
 * @param x - field's x coordinate of object.
 * @param y - field's y coordinate of object.
 */
void Dialog::createObject(const uint id, const int x, const int y, const QColor color) {
	int x_pos = calculateX(x);
	int y_pos = calculateY(y);
	int radius =  calculateRadius();
	new RoundObject(id, x_pos, y_pos, radius, scene, &timer, color);
}

/**
 * @brief RoundObject object search method.
 * @param id - object's id.
 * @return pointer to RoundObject. If there is RoundObject with passed id, nullptr is returned.
 */
RoundObject* Dialog::searchObject(const uint id) {
	QList<QGraphicsItem *> obj_list = scene->items();
	RoundObject *ptr;
	for (auto it : obj_list) {
		ptr = dynamic_cast<RoundObject *>(it);//TODO: why not use of pattern? Visitor or strategy maybe?
		if (ptr) {
			if(ptr->id() == id)
				return ptr;
		}
	}
	return nullptr;
}

/**
 * @brief Move object method with relative coordinates.
 *
 * By relative coordinates it is meant to be in relation to present object's position.
 * @param id - id of RoundObject which will be moved.
 * @param x - <b>realative field (not graphical) steps</b> in x direction to make.
 * @param y - <b>realative field (not graphical) steps</b> in y direction to make.
 */
void Dialog::moveObject(const uint id, const int x, const int y) {
	RoundObject *object = searchObject(id);
	if (!object) return; //TODO: throw exception
	if(!timer.isActive())
		timer.start(ANIMATION_CLOCK);
	object->move(calculateX(x), calculateY(y));
}

/**
 * @brief Move object method to specific field's coordinate.
 * @param id - id of RoundObject that will be moved.
 * @param x - <b>real field's x coordinate</b> in which RoundObject will be placed.
 * @param y - <b>real field's y coordinate</b> in which RoundObject will be placed.
 */
void Dialog::moveObjectTo(const uint id, const int x, const int y) {
	RoundObject *object = searchObject(id);
	int x_old, y_old, dx, dy;
	if (!object) return; //TODO: throw exception
	x_old = object->x();
	y_old = object->y();
	dx = calculateX(x) - x_old;
	dy = calculateY(y) - y_old;
	if(!timer.isActive())
		timer.start(ANIMATION_CLOCK);
	object->move(dx, dy);
}

/**
 * @brief Remove (delete) specific RoundObject.
 * @param id - id of RoundObject to be deleted.
 */
void Dialog::removeObject(const uint id) {
	RoundObject *object = searchObject(id);
	if (!object) return; //TODO: throw exception
	scene->removeItem(object);
	delete(object);
}



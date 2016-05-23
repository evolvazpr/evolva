#include "RoundObject.hpp"

/**
 * @brief INCREMENT_PER_TICK static global variable. Parameter to set speed of animation.
 */
static const qreal INCREMENT_PER_TICK = 3;

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
			QObject(scene->parent()), QGraphicsEllipseItem(0, 0, radius, radius),
		       	id_(id), timer_(timer) 
{
	setBrush(QBrush(color, Qt::SolidPattern));
	scene->addItem(this);
	dx_ = 0;
	dy_ = 0;
	setPos(x, y);	
}

void RoundObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
	qreal pos = rect().height();
	pos = pos/2.0;
	QString text = QString::number(id());
	QFontMetrics font(painter->font());
	qreal x = pos - ((qreal)font.width(text)/2.0);
	qreal y = pos - (qreal)font.height()/2.0;
	QRect rect = QRect(x, y, font.width(text), font.height()); 
	QGraphicsEllipseItem::paint(painter, option, widget);
	painter->drawText(rect, text);
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

int RoundObject::CalculateCoord(int *increment, int actual_coord) {
	int new_coord;
	if (*increment > 0) {
		if (*increment >= INCREMENT_PER_TICK) {
			*increment -= INCREMENT_PER_TICK;
			new_coord = actual_coord + INCREMENT_PER_TICK;
		} else {
			new_coord = actual_coord + *increment;
			*increment = 0;
		}
	} else if (*increment < 0) {
		if (std::abs(*increment) >= INCREMENT_PER_TICK) {
			*increment += INCREMENT_PER_TICK;
			new_coord = actual_coord - INCREMENT_PER_TICK;
		} else {
			new_coord = actual_coord + *increment;
			*increment = 0;
		}
	} else {
		new_coord = actual_coord;
	}
	return new_coord;
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
	int x_coord;
	int y_coord;

	x_coord = CalculateCoord(&dx_, x());
	y_coord = CalculateCoord(&dy_, y());

	setPos(x_coord, y_coord);

	if((!dx_) && (!dy_)) {
		QObject::disconnect(timer_, SIGNAL(timeout()), this, SLOT(animate()));
		emit AnimationFinished();
	}
}

bool RoundObject::IsMoving()
{
	bool ret = false;
		if (dx_ || dy_)
			ret = true;
	return ret;
}


#include <iostream>
SpriteObject::SpriteObject(const uint id, const int x, const int y,
			QGraphicsScene *scene, QTimer *timer, QString sprite_path) : 
			QObject(scene->parent()), QGraphicsPixmapItem(),
		       	id_(id), timer_(timer) 
{
	QPixmap pixmap(sprite_path);
	pixmap = pixmap.scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	setPixmap(pixmap);		
	scene->addItem(this);
	dx_ = 0;
	dy_ = 0;
	setPos(x, y);
}

/**
 * @brief RoundObject's deconstructor.
 */
SpriteObject::~SpriteObject() {
}


/**
 * @brief Object move method.
 *
 * It makes connection of timer's signals and object animate method.
 *
 * @param dx - how many steps in pixels to take into x direction.
 * @param dy - how many steps in pixels to take into y direction.
 */
void SpriteObject::move(const int dx, const int dy) {
	dx_ = dx;
	dy_ = dy;
	QObject::connect(timer_, SIGNAL(timeout()), this, SLOT(animate()));
}

/**
 * @brief Method to obtain id number.
 * @return object's id number.
 */
uint SpriteObject::id() {
	return id_;
}

int SpriteObject::CalculateCoord(int *increment, int actual_coord) {
	int new_coord;
	if (*increment > 0) {
		if (*increment >= INCREMENT_PER_TICK) {
			*increment -= INCREMENT_PER_TICK;
			new_coord = actual_coord + INCREMENT_PER_TICK;
		} else {
			new_coord = actual_coord + *increment;
			*increment = 0;
		}
	} else if (*increment < 0) {
		if (std::abs(*increment) >= INCREMENT_PER_TICK) {
			*increment += INCREMENT_PER_TICK;
			new_coord = actual_coord - INCREMENT_PER_TICK;
		} else {
			new_coord = actual_coord + *increment;
			*increment = 0;
		}
	} else {
		new_coord = actual_coord;
	}
	return new_coord;
}

/**
 * @brief Centerpiece of animation.
 *
 * This method is called when timer emits signal timeout().
 * It updates steps (dx, dy) that object must do, and updates
 * graphical coordinates of object. If there is no steps to perform,
 * method disconnects timer from this method.
 */
void SpriteObject::animate() {
	int x_coord;
	int y_coord;

	x_coord = CalculateCoord(&dx_, x());
	y_coord = CalculateCoord(&dy_, y());

	setPos(x_coord, y_coord);

	if((!dx_) && (!dy_)) {
		QObject::disconnect(timer_, SIGNAL(timeout()), this, SLOT(animate()));
		emit AnimationFinished();
	}
}

bool SpriteObject::IsMoving()
{
	bool ret = false;
		if (dx_ || dy_)
			ret = true;
	return ret;
}



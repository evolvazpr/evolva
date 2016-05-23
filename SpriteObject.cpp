#include "SpriteObject.hpp"

/**
 * @brief INCREMENT_PER_TICK static global variable. Parameter to set speed of animation.
 */
static const qreal INCREMENT_PER_TICK = 5.0;

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

SpriteObject::SpriteObject(const uint id, const int x, const int y,
			QGraphicsScene *scene, QTimer *timer, const QString sprite_path,
			const int sprites_cnt) : 
			QObject(scene->parent()), QGraphicsPixmapItem(),
		       	id_(id), timer_(timer), sprites_cnt_(sprites_cnt) 
{
	actual_sprite_ = 0;
	QPixmap pixmap(sprite_path);
	pixmap = pixmap.scaled(50*sprites_cnt, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	setPixmap(pixmap);	
	scene->addItem(this);
	dx_ = 0;
	dy_ = 0;
	setPos(x, y);
}


void SpriteObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
		           QWidget *widget) {
	QPixmap pmap = pixmap();
	QRectF target(0, 0, 50, 50);
	QRectF source(actual_sprite_*50, 0, 50, 50);
	painter->drawPixmap(target, pmap, source);
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

template <class T> T SpriteObject::CalculateCoord(T *lasts, T increment, T actual_coord) {
	T new_coord;
	if (qFabs(increment) <= qFabs(*lasts)) {
		*lasts -= increment;
		new_coord = actual_coord + increment;
	} else {
		new_coord = actual_coord + *lasts;
		*lasts = 0;
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
	qreal x_coord = 0;
	qreal y_coord = 0;
	qreal angle = 0;
	qreal dx = 0, dy = 0;

	angle = qAtan(qFabs((qreal)dx_)/qFabs((qreal)dy_));
	dx = INCREMENT_PER_TICK * qSin(angle);
	dy = INCREMENT_PER_TICK * qCos(angle);

	if (dx_ < 0)
		dx *= -1.0;
	if (dy_ < 0)
		dy *= -1.0;

	x_coord = CalculateCoord(&dx_, dx, x()); 
	y_coord = CalculateCoord(&dy_, dy, y());

	setPos(x_coord, y_coord);
	actual_sprite_ = (actual_sprite_ + 1) % (sprites_cnt_ - 1);
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



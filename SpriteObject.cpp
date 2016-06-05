#include "SpriteObject.hpp"
#include <iostream>
#include <QThread>
/**
 * @brief INCREMENT_PER_TICK static global variable. Parameter to set speed of animation.
 */
static const qreal INCREMENT_PER_TICK = 5.0;

/**
 * @brief SpriteObject constructor.
 * @param id - object's id.
 * @param x - object's graphical x coordinate (it is not field coordinate!).
 * To calculate graphical coordinates use Dialog::calculateX method.
 * @param y - object's graphical y coordinate (it is not field coordinate!).
 * To calculate graphical coordinates use Dialog::calculateY method.
 * @param radius - object's radius.
 * To calculate graphical radius use Dialog::calculateRadius method.
 * @param scene - pointer to scene.
 * @param timer - pointer to timer. It is needed, because on each move there is
 * established connection between timer signal timout() and SpriteObject slot animate().
 * @param sprites_cnt - describes dimension of sprite's matrix.
 * @param pixsize - describes graphics cell size
 */

SpriteObject::SpriteObject(QObject * parent, const uint id, const int x, const int y, 
		          const QString sprite_path, const int sprites_cnt, const uint pixsize) : 
			  QObject(parent), QGraphicsPixmapItem(),
		       	  id_(id), sprites_cnt_(sprites_cnt),
			  pixsize_(pixsize)	
{
	actual_sprite_ = 0;
	QPixmap pixmap(sprite_path);
	pixmap = pixmap.scaled(pixsize_ * sprites_cnt_, pixsize_ * sprites_cnt_, Qt::IgnoreAspectRatio, 
			       Qt::SmoothTransformation);
	setPixmap(pixmap);	
	dx_ = 0;
	dy_ = 0;
	direction_ = 0;
	actual_sprite_ = 0;
	setPos(x, y);
	setZValue(1);
}

/**
 * @brief Paint method which sets proper sprite to be drawn. Arguments not important for user.
 */
void SpriteObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
		           QWidget *widget) {
	QPixmap pmap = pixmap();
	QRectF target(0, 0, pixsize_, pixsize_);
	QRectF source(actual_sprite_ * pixsize_, pixsize_ * direction_, pixsize_, pixsize_);
	
	painter->drawPixmap(target, pmap, source);
	(void)option;			//silencing "not used parameter" warning.
	(void)widget;			//silencing "not used parameter" warning.
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
}

/**
 * @brief Method to obtain id number.
 * @return object's id number.
 */
uint SpriteObject::id() {
	return id_;
}


/**
 * @brief meta-method used by animate() method to get new position of object.
 * @param lasts - describes distance to final place of object.
 * @param increment - describes single size of step, which is made per animate() call.
 * @param actual_cord - describes actual coordinate of graphic object.
 */
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

	if((!dx_) && (!dy_)) 
		return;
	angle = qAtan(qFabs((qreal)dx_)/qFabs((qreal)dy_));
	dx = INCREMENT_PER_TICK * qSin(angle);
	dy = INCREMENT_PER_TICK * qCos(angle);

	if (dx_ < 0)
		dx *= -1.0;
	if (dy_ < 0)
		dy *= -1.0;

	x_coord = CalculateCoord(&dx_, dx, x()); 
	y_coord = CalculateCoord(&dy_, dy, y());

	/* direction designation */

	if (qFabs(dx_) > qFabs(dy_)) {
		if (dx_ < 0)
			direction_ = 1;
		else if (dx_ > 0)
			direction_ = 2;	
	} else {
		if (dy_ < 0)
			direction_ = 3;
		else if (dy_ > 0) 
			direction_ = 0;
	}
	
	
	actual_sprite_ = (actual_sprite_ + 1) % (sprites_cnt_ - 1);
	
	if ((!dx_ && !dy_))
		actual_sprite_ = 0;

	setPos(x_coord, y_coord);
	if((!dx_) && (!dy_)) {
		emit AnimationFinished();
	}
}

/**
 * @brief checks whether object has animation to perform.
 *
 * @ret returns true if animation is on-going, false if not.
 */  
bool SpriteObject::IsMoving()
{
	bool ret = false;
		if (dx_ || dy_)
			ret = true;
	return ret;
}

/**
 * @brief Method called when SpriteObject was clicked by mouse.
 * Method used only internally.
 */
void SpriteObject::mousePressEvent(QGraphicsSceneMouseEvent *event) {
	int x_pos = qFloor(x()/(qreal)pixsize_ + 0.5);
	int y_pos = qFloor(y()/(qreal)pixsize_ + 0.5);
	emit wasClicked(x_pos, y_pos);
	(void)event;
}

/**
 * @brief Method returns bounding rectangle of an object.
 */
QRectF SpriteObject::boundingRect() const {
	return QRectF(0, 0, pixsize_, pixsize_);
}

/**
 * @brief Override of QGraphicsPixmapItem method.
 * It is needed by Dialog::SearchObject.
 */ 
int SpriteObject::type() const {
	return Type;
}

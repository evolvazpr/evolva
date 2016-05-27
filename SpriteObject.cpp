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
			const int sprites_cnt, const uint pixsize) : 
			QObject(scene->parent()), QGraphicsPixmapItem(),
		       	id_(id), timer_(timer), sprites_cnt_(sprites_cnt),
			pixsize_(pixsize)	
{
	actual_sprite_ = 0;
	QPixmap pixmap(sprite_path);
	pixmap = pixmap.scaled(pixsize_ * sprites_cnt_, pixsize_ * sprites_cnt_, Qt::IgnoreAspectRatio, 
			       Qt::SmoothTransformation);
	setPixmap(pixmap);	
	scene->addItem(this);
	dx_ = 0;
	dy_ = 0;
	direction_ = 0;
	actual_sprite_ = 0;
	setPos(x, y);
}


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

void SpriteObject::mousePressEvent(QGraphicsSceneMouseEvent *event) {
	int x_pos = x()/pixsize_;
	int y_pos = y()/pixsize_;
	emit wasClicked(x_pos, y_pos);
	(void)event;
}

QRectF SpriteObject::boundingRect() const {
	return QRectF(0, 0, pixsize_, pixsize_);
}

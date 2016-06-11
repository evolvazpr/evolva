#ifndef _ROUNDOBJECT_HPP_
#define _ROUNDOBJECT_HPP_

#include <QObject>
#include <QGraphicsObject>
#include <QGraphicsEllipseItem>
#include <QTimer>
#include <QBrush>
#include <QColor>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsScene>
#include <QPainter>
#include <QtMath>
#include <QMutex>

/**
 * @brief Implementation of sprite image graphic representation.
 */
class SpriteObject : public QObject, public QGraphicsPixmapItem {
	Q_OBJECT
private:
	qreal steps_per_tick_;
	int id_;
	qreal dx_;
	qreal dy_;
	int sprites_cnt_;
	int actual_sprite_;
	int direction_;
	const uint pixsize_;
	qreal CalculateCoord(qreal *lasts, qreal increment, qreal actual_coord);

protected:
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;

public:
	SpriteObject(QObject* parent, const uint pixsize);
	void SetObject(QObject* parent, const uint id, const qreal x, const qreal y,
		  const QPixmap& pixmap, const int sprites_cnt);

	virtual ~SpriteObject();
	void Move(const qreal x, const qreal y, const qreal steps_per_tick);
	uint GetId();
	bool IsMoving();
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
		           QWidget *wdiget) Q_DECL_OVERRIDE;
	virtual QRectF boundingRect() const Q_DECL_OVERRIDE;

	enum {Type = UserType + 1};
	virtual int type() const Q_DECL_OVERRIDE;
signals:
	void AnimationFinished();
	void WasClicked(int x, int y);
public slots:
	void Animate();
};

#endif //_ROUNDOBJECT_HPP_

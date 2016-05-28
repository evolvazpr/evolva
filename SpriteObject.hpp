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
/**
 * @brief Implementation of sprite image graphic representation.
 */
class SpriteObject : public QObject, public QGraphicsPixmapItem {
	Q_OBJECT
private:
	const int id_;
	qreal dx_;
	qreal dy_;
	QTimer *timer_;
	const int sprites_cnt_;
	int actual_sprite_;
	int direction_;
	const uint pixsize_;
	template <class T> T CalculateCoord(T *lasts, T increment, T actual_coord);

protected:
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;

public:
	SpriteObject(const uint id, const int x, const int y, QGraphicsScene *scene, 
		     QTimer *timer, QString path, const int sprites_cnt, const uint pixsize);
	virtual ~SpriteObject();
	void move(const int x, const int y);
	uint id();
	bool IsMoving();
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
		           QWidget *wdiget) Q_DECL_OVERRIDE;
	virtual QRectF boundingRect() const Q_DECL_OVERRIDE;
signals:
	void AnimationFinished();
	void wasClicked(int x, int y);
public slots:
	void animate();
};

#endif //_ROUNDOBJECT_HPP_

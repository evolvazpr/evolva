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

/**
 * @brief The RoundObject class (GUI part).
 */
class RoundObject : public QObject, public QGraphicsEllipseItem {
	Q_OBJECT
private:
	const int id_;
	int dx_;
	int dy_;
	QTimer *timer_;

	int CalculateCoord(int *increment, int actual_coord);
public:
	RoundObject(const uint id, const int x, const int y, const uint radius, 
		    QGraphicsScene *scene, QTimer *timer, QColor color);
	virtual ~RoundObject();
	void move(const int x, const int y);
	uint id();
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	bool IsMoving();
signals:
	void AnimationFinished();
public slots:
	void animate();
};

#endif //_ROUNDOBJECT_HPP_

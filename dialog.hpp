#ifndef DIALOG_HPP
#define DIALOG_HPP

#include <QDialog>
#include <QGraphicsObject>
#include <QTimer>
#include <QColor>
#include <QBrush>
#include <QStyleOptionGraphicsItem>
#include <cmath>
#include <iostream>
#include <QPainter>
#include <memory>
#include <QMutex>

class Field;
class CellObject;

namespace Ui {
	class Dialog;
}

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
public:
	RoundObject(const uint id, const int x, const int y, const uint radius, 
		    QGraphicsScene *scene, QTimer *timer, QColor color);
	virtual ~RoundObject();
	void move(const int x, const int y);
	uint id();
signals:
	void AnimationFinished();
public slots:
	void animate();
};

/**
 * @brief The Dialog class (GUI part)
 */
class Dialog : public QDialog
{
	Q_OBJECT

private:
	Ui::Dialog *ui;
	QGraphicsScene *scene;
	const unsigned int width_;
	const unsigned int height_;

	RoundObject* SearchObject(const uint id);
	int calculateX(const int x);
	int calculateY(const int y);
	uint calculateRadius();
	QList<QGraphicsItem *> to_remove_;
	QTimer timer;
	QAtomicInt animations_;
	void RemoveObject(const uint id);
public:
	explicit Dialog(QWidget *parent = 0);
	virtual ~Dialog();

	void CreateObject(std::shared_ptr<const CellObject> object, const int x, const int y);
	void MoveObject(std::shared_ptr<const CellObject> object, const int x, const int y);
	void MoveObjectTo(std::shared_ptr<const CellObject> object, const int x, const int y);
	void RemoveObject(std::shared_ptr<const CellObject> object);
	void ClearField();
private slots:
	void on_pushButton_clicked();

public slots:
	void AnimationFinished();
};

#endif // DIALOG_HPP

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

	RoundObject* searchObject(const uint id);
	int calculateX(const int x);
	int calculateY(const int y);
	uint calculateRadius();

	QTimer timer;


public:
	explicit Dialog(QWidget *parent = 0);
	virtual ~Dialog();

	void createObject(const uint id, const int x, const int y, const QColor color);
	void moveObject(const uint id, const int x, const int y);
	void moveObjectTo(const uint id, const int x, const int y);
	void removeObject(const uint id);

private slots:
	void on_pushButton_clicked();
};

#endif // DIALOG_HPP

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
#include <memory>
#include <QMutex>
#include <unordered_map>
#include "SpriteObject.hpp"

namespace Ui {
	class Dialog;
}

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
	QTimer timer_;
	QAtomicInt animations_;
	QList<SpriteObject *> to_add_;
	QList<SpriteObject *> to_remove_;	
	SpriteObject* SearchObject(const uint id);
	qreal CalculateX(const int x);
	qreal CalculateY(const int y);
public:

	explicit Dialog(QWidget *parent = 0, const int width = 1, const int height = 1);

	virtual ~Dialog();
	void CreateObject(const uint id, QString path, uint sprite_cnt, const int x, const int y);
	void CreateSurfaceObject(const QString path, const int x, const int y);
	void RemoveSurfaceObject(const int x, const int y);
	void MoveObject(const uint id, const int x, const int y);
	void MoveObjectTo(const uint id, const int x, const int y);
	void RemoveObject(const uint id);
	void UpdateStats(const QString text);
	void UpdateLog(const QString text); 
private slots:
	void on_pushButton_clicked();
	void AnimationFinished();
signals:
	void NextLogicIteration();
	void SpriteObjectClicked(int x, int y);
};

#endif // DIALOG_HPP

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
#include <QWaitCondition>
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
	static Dialog* dialog_;

	SpriteObject* SearchObject(const uint id);
	qreal CalculateX(const int x);
	qreal CalculateY(const int y);
	explicit Dialog(QWidget *parent = 0, const int width = 1, const int height = 1);

public:
	/**
	 * @brief enum class to describe possible surface types.
	 */
	
	virtual ~Dialog();
	static Dialog * GetInstance(QWidget *parent = 0, const int width = 1, const int height = 1);
	void AppendTextToLog(const QString text);
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

signals:
	void ClearMutex();
	void NextLogicIteration();
	void SpriteObjectClicked(int x, int y);
};

#endif // DIALOG_HPP

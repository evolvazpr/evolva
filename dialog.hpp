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

#include "XmlIo.hpp"

class RoundObject;
class Field;
class CellObject;
class SpriteObject;

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
	XmlIo sprites;
	Ui::Dialog *ui;
	QGraphicsScene *scene;
	const unsigned int width_;
	const unsigned int height_;

	SpriteObject* SearchObject(const uint id);
	int calculateX(const int x);
	int calculateY(const int y);
	uint calculateRadius();
	QList<QGraphicsItem *> to_remove_;
	QTimer timer;
	QAtomicInt animations_;
	void RemoveObject(const uint id);
	void IncrementAnimations(SpriteObject *roundObject);

public:
	enum class Ground {
		WATER,
		SAND,
		GRASS,
		SOIL		
	};

	explicit Dialog(QWidget *parent = 0);
	virtual ~Dialog();

	void CreateObject(std::shared_ptr<const CellObject> object, const int x, const int y);
	void CreateGroundObject(const Dialog::Ground ground_type, const int x, const int y);
	void RemoveGroundObject(const int x, const int y);
	void MoveObject(std::shared_ptr<const CellObject> object, const int x, const int y);
	void MoveObjectTo(std::shared_ptr<const CellObject> object, const int x, const int y);
	void RemoveObject(std::shared_ptr<const CellObject> object);
	void AppendTextToLog(std::string text);
	void ClearField();

	friend Dialog& operator <<(Dialog& dialog, std::string s);
	friend Dialog& operator <<(Dialog& dialog, const char* s);

private slots:
	void on_pushButton_clicked();

public slots:
	void AnimationFinished();
	void SpriteObjectClicked(int x, int y);
};

#endif // DIALOG_HPP

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
#include <boost/format.hpp>

#include "XmlIo.hpp"

class RoundObject;
class Field;
class CellObject;
class SpriteObject;
class FieldCell;

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
	QList<QGraphicsItem *> to_remove_;
	QTimer timer;
	QAtomicInt animations_;
	QMutex remove_mutex_;

	SpriteObject* SearchObject(const uint id);
	qreal CalculateX(const int x);
	qreal CalculateY(const int y);
	void RemoveObject(const uint id);
	void IncrementAnimations(SpriteObject *roundObject);
	boost::format CreateStatistics(std::shared_ptr<FieldCell> cell);

public:
	/**
	 * @brief enum class to describe possible surface types.
	 */
	enum class Surface {
		WATER,
		SAND,
		GRASS,
		SOIL		
	};

	explicit Dialog(QWidget *parent = 0);
	virtual ~Dialog();

	void CreateObject(std::shared_ptr<const CellObject> object, const int x, const int y);
	void CreateSurfaceObject(const Dialog::Surface ground_type, const int x, const int y);
	void RemoveSurfaceObject(const int x, const int y);
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

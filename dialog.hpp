#ifndef DIALOG_HPP
#define DIALOG_HPP

#include <QGui>
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
#include <unordered_map>
#include "XmlIo.hpp"
#include "CellObject.hpp"

class RoundObject;
class Field;
class SpriteObject;
class FieldCell;

namespace Ui {
	class Gui;
}

/**
 * @brief The Gui class (GUI part)
 */
class Gui : public QDialog
{
	Q_OBJECT

private:
	XmlIo sprites;
	Ui::Gui *ui;
	QGraphicsScene *scene;
	const unsigned int width_;
	const unsigned int height_;
	QList<QGraphicsItem *> to_remove_;
	QTimer timer;
	QAtomicInt animations_;
	QMutex remove_mutex_;

	static Gui * gui_;

	SpriteObject* SearchObject(const uint id);
	qreal CalculateX(const int x);
	qreal CalculateY(const int y);
	void RemoveObject(const uint id);
	void IncrementAnimations(SpriteObject *roundObject);
	boost::format CreateStatistics(std::shared_ptr<FieldCell> cell);
	std::string GetTypeName(std::shared_ptr<const CellObject> object); //it should be somewhere else

	explicit Gui(QWidget *parent = 0);

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
	virtual ~Gui();
	static Gui * GetInstance(QWidget *parent = 0);
	void CreateObject(std::shared_ptr<const CellObject> object, const int x, const int y);
	void CreateSurfaceObject(const Gui::Surface ground_type, const int x, const int y);
	void RemoveSurfaceObject(const int x, const int y);
	void MoveObject(std::shared_ptr<const CellObject> object, const int x, const int y);
	void MoveObjectTo(std::shared_ptr<const CellObject> object, const int x, const int y);
	void RemoveObject(std::shared_ptr<const CellObject> object);
	void AppendTextToLog(std::string text);
	void ClearField();

	friend Gui& operator <<(Dialog& gui, const std::string s);
	friend Gui& operator <<(Dialog& gui, const char* s);

private slots:
	void on_pushButton_clicked();

public slots:
	void AnimationFinished();
	void SpriteObjectClicked(int x, int y);
};

#endif // DIALOG_HPP

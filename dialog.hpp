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
#include <boost/format.hpp>
#include <unordered_map>
#include "XmlIo.hpp"
#include "CellObject.hpp"
#include <QThread>
#include "SpriteObject.hpp"


class RoundObject;
class Field;
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
	XmlIo settings_;
	Ui::Dialog *ui;
	QGraphicsScene *scene;
	const unsigned int width_;
	const unsigned int height_;
	QThread gui_thread_;
	QList<QGraphicsItem *> to_remove_;
	QTimer timer_;
	QAtomicInt animations_;
	QMutex gui_mutex_;
	static Dialog* dialog_;

	SpriteObject* SearchObject(const uint id);
	qreal CalculateX(const int x);
	qreal CalculateY(const int y);
	void RemoveObject(const uint id);
	void IncrementAnimations(SpriteObject *roundObject);
	boost::format CreateStatistics(std::shared_ptr<FieldCell> cell);
	std::string GetTypeName(std::shared_ptr<const CellObject> object); //it should be somewhere else

	explicit Dialog(QWidget *parent = 0, const int width = 1, const int height = 1);

public:
	/**
	 * @brief enum class to describe possible surface types.
	 */
	
	virtual ~Dialog();
	static Dialog * GetInstance(QWidget *parent = 0, const int width = 1, const int height = 1);
	void CreateObject(std::shared_ptr<const CellObject> object, const int x, const int y);
	void CreateSurfaceObject(const FieldCell::Ground ground_type, const int x, const int y);
	void RemoveSurfaceObject(const int x, const int y);
	void MoveObject(std::shared_ptr<const CellObject> object, const int x, const int y);
	void MoveObjectTo(std::shared_ptr<const CellObject> object, const int x, const int y);
	void RemoveObject(std::shared_ptr<const CellObject> object);
	void AppendTextToLog(std::string text);
	void UpdateField();

	friend Dialog& operator <<(Dialog& dialog, const std::string s);
	friend Dialog& operator <<(Dialog& dialog, const char* s);

private slots:
	void on_pushButton_clicked();

public slots:
	void AnimationFinished();
	void SpriteObjectClicked(int x, int y);
};

#endif // DIALOG_HPP

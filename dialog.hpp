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
	Ui::Dialog *ui;
	QGraphicsScene *scene;
	const unsigned int width_;
	const unsigned int height_;
	QTimer timer_;
	static Dialog* dialog_;

	SpriteObject* SearchObject(const uint id);
	qreal CalculateX(const int x);
	qreal CalculateY(const int y);
	boost::format CreateStatistics(std::shared_ptr<FieldCell> cell);
	explicit Dialog(QWidget *parent = 0, const int width = 1, const int height = 1);

public:
	/**
	 * @brief enum class to describe possible surface types.
	 */
	
	virtual ~Dialog();
	static Dialog * GetInstance(QWidget *parent = 0, const int width = 1, const int height = 1);
	void AppendTextToLog(std::string text);

	friend Dialog& operator <<(Dialog& dialog, const std::string s);
	friend Dialog& operator <<(Dialog& dialog, const char* s);

	void CreateObject(const uint id, QString path, uint sprite_cnt, const int x, const int y);
	void CreateSurfaceObject(const QString path, const int x, const int y);
private slots:
	void on_pushButton_clicked();
	void RemoveSurfaceObject(const int x, const int y);
	void MoveObject(const uint id, const int x, const int y);
	void MoveObjectTo(const uint id, const int x, const int y);
	void RemoveObject(const uint id);
public slots:
	void AnimationFinished();
	void SpriteObjectClicked(int x, int y);

signals:
	void ClearMutex();
	void NextLogicIteration();	
};

#endif // DIALOG_HPP

#ifndef DIALOG_HPP
#define DIALOG_HPP

#include <QDialog>
#include <QGraphicsObject>
#include <QTimer>
#include <QColor>
#include <QBrush>
#include <QStyleOptionGraphicsItem>
#include <QMutex>
#include <unordered_map>
#include <QScrollBar>
#include <QMessageBox>

#include "ui_dialog.h"
#include "SpriteObject.hpp"
#include "Field.hpp"
#include "EvolvaException.hpp"
#include "Statistics.hpp"

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
	
	const qreal animation_clock_;
	const uint pixels_per_object_;
	uint steps_per_tick_;
	uint count_of_rounds_;
	
	QGraphicsScene *scene;
	const unsigned int width_;
	const unsigned int height_;
	QTimer timer_;
	QAtomicInt animations_;

	QList<SpriteObject *> to_add_;
	QList<SpriteObject *> to_remove_;
	QList<SpriteObject *> sprite_object_pool_;

	SpriteObject* SearchObject(const uint id) const;
	qreal CalculateX(const int x) const;
	qreal CalculateY(const int y) const;
public:

	explicit Dialog(QWidget *parent = 0, const int width = 1, const int height = 1, 
			const int pixels_per_object = 25);
	virtual ~Dialog();
	void CreateObject(const uint id, const QPixmap& pixmap, uint sprite_cnt, const int x, const int y);
	void CreateSurfaceObject(const QPixmap& pixmap, const int x, const int y);
	void RemoveSurfaceObject(const int x, const int y);
	void ReplaceSurfaceObject(const QPixmap& pixmap, const int x, const int y);
	void MoveObject(const uint id, const int x, const int y);
	void MoveObjectTo(const uint id, const int x, const int y);
	void RemoveObject(const uint id);
	void UpdateStats(const QString text);
	void UpdateLog(const QString text); 
	void UpdateOverallStatistics();

private slots:
	//Names of slots are generated by qt. This is why there are different than coding-style which is used.
	void on_pushButton_move_clicked();
	void on_pushButton_round_clicked();
	void on_pushButton_settings_clicked();
	
	void AnimationFinished();
signals:
	void NextLogicIteration();
	void MoveToTheEndOfRound(uint rounds);
	void SpriteObjectClicked(int x, int y);
};

#endif // DIALOG_HPP

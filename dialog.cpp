#include "dialog.hpp"
#include "ui_dialog.h"
#include "Field.hpp"
#include "EvolvaException.hpp"
#include "Unit.hpp"

/**
 * @brief Constructor.
 * @param parent - parent for Qt API (no need to call delete thanks to it).
 */
Dialog::Dialog(QWidget *parent, const int width, const int height) :
	QDialog(parent), ui(new Ui::Dialog), animation_clock_(1000.0/33.0), pixels_per_object_(25), steps_per_tick_(5.0),  width_(width), height_(height) { 
	Qt::WindowFlags flags = Qt::WindowTitleHint | Qt::WindowSystemMenuHint;
	flags |= Qt::WindowCloseButtonHint | Qt::WindowMinMaxButtonsHint;
	
	ui->setupUi(this);
	scene = new QGraphicsScene();
	ui->graphicsView->setScene(scene);
	scene->setSceneRect(0, 0, width_ * pixels_per_object_, height_ * pixels_per_object_);
	ui->graphicsView->centerOn(scene->sceneRect().center());
	ui->graphicsView->setCacheMode(QGraphicsView::CacheNone);
	ui->graphicsView->setRenderHint(QPainter::Antialiasing);
	ui->graphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
	ui->graphicsView->show();
	animations_ = 0;
	QDialog::setWindowFlags(flags);
	timer_.start(animation_clock_);	
	rounds_per_click_ = ui->lineEdit_rounds->text().toUInt();
	steps_per_tick_ = ui->lineEdit_steps->text().toUInt();
}

/**
 * @brief Deconstructor.
 */
Dialog::~Dialog() {
	delete ui;
}

/**
 * @brief pushButton action method.
 * Method called when "Kolejna tura" button was clicked.
 */
void Dialog::on_pushButton_clicked() {
       for (uint i = 0; i < rounds_per_click_; ++i) 
		emit NextLogicIteration();
}

void Dialog::on_pushButton_2_clicked() {
	bool test;
	const uint rounds = ui->lineEdit_rounds->text().toUInt(&test, 10);
	if (!test) {
		return;
	}
	const uint steps = ui->lineEdit_steps->text().toUInt(&test, 10);
	if (!test) {
		return;
	}
	rounds_per_click_ = rounds;
	steps_per_tick_ = steps;
}
/**
 * @brief Graphical x coordinate calculation.
 * @param x - field's x coordinate.
 * @return  - graphical x coordinate.
 */
qreal Dialog::CalculateX(const int x) {
	return ((qreal)x) / ((qreal)width_ )* scene->width();
}

/**
 * @brief Graphical y coordinate calculation.
 * @param y - field's y coordinate.
 * @return  - graphical y coordinate.
 */
qreal Dialog::CalculateY(const int y) {
	return ((qreal)y) / ((qreal)height_) * scene->height();
}


/**
 * @brief RoundObject object creation.
 * To get better scalability GetTypeName method was created, but it is not the best idea.
 * @param id - object's id.
 * @param x - field's x coordinate of object.
 * @param y - field's y coordinate of object.
 */
void Dialog::CreateObject(const uint id, QString path, uint sprite_cnt, const int x, const int y) {
	QMutexLocker lock(&mutex_);
	qreal x_pos = CalculateX(x);
	qreal y_pos = CalculateY(y);
	SpriteObject *sprite_object;
	std::string obj_type;
	QString sprite_path;
	sprite_object = new SpriteObject(scene->parent(), id, x_pos, y_pos, 
					path, sprite_cnt, pixels_per_object_);
	if (animations_.fetchAndAddAcquire(0))
		to_add_.push_back(sprite_object);
	else 
		scene->addItem(sprite_object);
	
	QObject::connect(dynamic_cast<QObject *>(sprite_object), SIGNAL(AnimationFinished()), 
			this, SLOT(AnimationFinished()));
	QObject::connect(dynamic_cast<QObject *>(sprite_object), SIGNAL(WasClicked(int, int)), this, 
			SIGNAL(SpriteObjectClicked(int, int)));
	QObject::connect(&timer_, SIGNAL(timeout()), dynamic_cast<QObject *>(sprite_object), 
			SLOT(Animate()));
}

void Dialog::AnimationFinished() {
	QMutexLocker lock(&mutex_);
	animations_.fetchAndAddAcquire(-1);
	if (!animations_.fetchAndAddAcquire(0)) {
		for (auto &it : to_add_) {
			scene->addItem(it);
		}
		to_add_.clear();

		for (auto &it : to_remove_) {
			scene->removeItem(it);
			delete(it);
		}
		to_remove_.clear();
	}
}

/**
 * @brief SpriteObject object search method.
 * @param id - object's id.
 * @return pointer to SpriteObject. If there is no SpriteObject with passed id, nullptr is returned.
 */
SpriteObject* Dialog::SearchObject(const uint id) {
	QList<QGraphicsItem *> obj_list = scene->items();
	SpriteObject *ptr;
	for (auto &it : obj_list) {
		if (it->type() != QGraphicsPixmapItem::Type) {
			ptr = dynamic_cast<SpriteObject *>(it); //How to not to cast?
			if (ptr == nullptr)
				throw EvolvaException("Dialog::SearchObject! IT SHOULD NOT OCCURE!");
			if(ptr->GetId() == id)
				return ptr;
		}
	}
	
	for (auto &ptr : to_add_) {
		if (ptr->GetId() == id) {
			return ptr;
		}
	}

	return nullptr;
}

/**
 * @brief Move object by relative coordinates (in other words - move with (x, y) steps somewhere).
 *
 * By relative coordinates it is meant to be in relation to present object's position.
 * @param object - shared_ptr to object which will be moved.
 * @param x - <b>realative field (not graphical) steps</b> in x direction to make.
 * @param y - <b>realative field (not graphical) steps</b> in y direction to make.
 */
void Dialog::MoveObject(const uint id, const int x, const int y) {
	QMutexLocker lock(&mutex_);
	SpriteObject *roundObject = SearchObject(id);
	if (!roundObject) {
		throw EvolvaException("Dialog::moveObject - object not found!\n");
	}
	if (!roundObject->IsMoving())
		animations_.fetchAndAddAcquire(1);
	
	roundObject->Move(CalculateX(x), CalculateY(y), steps_per_tick_);	
}

/**
 * @brief Move object method to specific field's coordinate.
 * @param object - shared_ptr of object that will be moved.
 * @param x - <b>real field's x coordinate</b> in which RoundObject will be placed.
 * @param y - <b>real field's y coordinate</b> in which RoundObject will be placed.
 */
void Dialog::MoveObjectTo(const uint id, const int x, const int y) {
	QMutexLocker lock(&mutex_);
	SpriteObject *roundObject = SearchObject(id);
	int x_old, y_old, dx, dy;
	if (!roundObject) {
		throw EvolvaException("Dialog::moveObjectTo - object not found!\n");
	}

	x_old = roundObject->x();
	y_old = roundObject->y();

	dx = CalculateX(x) - x_old;
	dy = CalculateY(y) - y_old;
	
	if (!roundObject->IsMoving())
		animations_.fetchAndAddAcquire(1);

	roundObject->Move(dx, dy, steps_per_tick_);
}

/**
 * @brief Remove (delete) specific SpriteObject.
 * @param object - shared_ptr to object which will be deleted from GUI.
 */
void Dialog::RemoveObject(const uint id) {
	QMutexLocker lock(&mutex_);
	SpriteObject *roundObject = SearchObject(id);

	if (!roundObject)
		throw EvolvaException("Dialog::removeObject - internal error!\n");		

	if (animations_.fetchAndAddAcquire(0)) {
		to_remove_.push_back(roundObject);
	} else {
		scene->removeItem(roundObject);
		delete(roundObject);
	}
}

/**
 * @brief Creation of graphic surface.
 *	
 * @param surface_type - Type of surface to create.
 * @param x - x coordinate.
 * @param y - y coordinate.
 */
void Dialog::CreateSurfaceObject(const QString path, const int x, const int y) {	
	QPixmap pix_map(path);
	if (pix_map.isNull())
		throw EvolvaException("Sprite could not have been loaded." 
			"Aborting program.\nCheck if gui.xml is correct.");
	
	pix_map = pix_map.scaled(pixels_per_object_, pixels_per_object_, 
				 Qt::IgnoreAspectRatio, 
				 Qt::SmoothTransformation);
	QGraphicsPixmapItem *rect = new QGraphicsPixmapItem(pix_map);
	rect->setOffset(CalculateX(x), CalculateY(y));
	scene->addItem(rect);
}

/**
 * @brief Deletion of graphic surface.
 * @param x - x coordinate.
 * @param y - y coordinate.
 */
void Dialog::RemoveSurfaceObject(const int x, const int y) {
	QTransform test;
	QGraphicsItem *item = scene->itemAt(CalculateX(x), CalculateY(y), test);
	if (!item)
		return;
	scene->removeItem(item);
	delete(item);	
}


/**
 * @brief Method to append text to log window.
 * @param text - text to append.
 */
void Dialog::UpdateStats(const QString text) {
	ui->stats_textWindow->setPlainText(text);
}

/**
 * @brief Method called when sprite object was clicked. 
 *
 * Method is invoked because of connection between SpriteObject Qt signal and this Qt slot.
 * Method used for text insertion into stats window. 
 *
 * @param x - x coordinate of sprite object.
 * @param y - y coordinate of sprite object.
 */
void Dialog::UpdateLog(const QString text) {
	ui->log_textWindow->insertPlainText(text);		
}


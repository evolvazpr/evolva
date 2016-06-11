#include "dialog.hpp"
#include "ui_dialog.h"
#include "Field.hpp"
#include "EvolvaException.hpp"
#include "Unit.hpp"
#include "Statistics.hpp"

/**
 * @brief Constructor.
 * @param parent - parent for Qt API (no need to call delete thanks to it).
 */
Dialog::Dialog(QWidget *parent, const int width, const int height, const int pixels_per_object) :
	QDialog(parent), ui(new Ui::Dialog), animation_clock_(1000.0/33.0), 
	pixels_per_object_(pixels_per_object), steps_per_tick_(5), count_of_rounds_(1),  width_(width), height_(height) { 
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
	if (animations_)
		return;
	emit NextLogicIteration();
}

void Dialog::on_pushButton_3_clicked() {
	if (animations_)
		return;
	emit MoveToTheEndOfRound(count_of_rounds_);	
}

void Dialog::on_pushButton_2_clicked() {
	bool test;
	const uint steps = ui->lineEdit_steps->text().toUInt(&test, 10);
	if (!test) {
		QMessageBox::warning(this, tr("Evolva"), tr("Wrong input data. Only numerics are allowed."));	
		return;
	}
	const uint rounds = ui->lineEdit_rounds->text().toUInt(&test, 10);
	if (!test) {
		QMessageBox::warning(this, tr("Evolva"), tr("Wrong input data. Only numerics are allowed."));	
		return;
	}
	steps_per_tick_ = steps;
	count_of_rounds_ = rounds;
}
/**
 * @brief Graphical x coordinate calculation.
 * @param x - field's x coordinate.
 * @return  - graphical x coordinate.
 */
qreal Dialog::CalculateX(const int x) {
	return qFloor(((qreal)x) / ((qreal)width_ )* scene->width() + 0.5);
}

/**
 * @brief Graphical y coordinate calculation.
 * @param y - field's y coordinate.
 * @return  - graphical y coordinate.
 */
qreal Dialog::CalculateY(const int y) {
	return qFloor(((qreal)y) / ((qreal)height_) * scene->height() + 0.5);
}


/**
 * @brief RoundObject object creation.
 * To get better scalability GetTypeName method was created, but it is not the best idea.
 * @param id - object's id.
 * @param x - field's x coordinate of object.
 * @param y - field's y coordinate of object.
 */
void Dialog::CreateObject(const uint id, const QPixmap& pixmap, uint sprite_cnt, const int x, const int y) {
	QMutexLocker lock(&mutex_);
	qreal x_pos = CalculateX(x);
	qreal y_pos = CalculateY(y);
	SpriteObject *sprite_object;
	std::string obj_type;
	QString sprite_path;
	sprite_object = new SpriteObject(scene->parent(), id, x_pos, y_pos, 
					pixmap, sprite_cnt, pixels_per_object_);
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
	if (!roundObject->IsMoving() && steps_per_tick_)
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
	
	if (!roundObject->IsMoving() && (steps_per_tick_ > 1.0))
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
void Dialog::CreateSurfaceObject(const QPixmap& pixmap, const int x, const int y) {		
	QGraphicsPixmapItem *rect = new QGraphicsPixmapItem(pixmap);
	if (rect == nullptr)//remove it?
		throw EvolvaException("?");
	scene->addItem(rect);
	rect->setOffset(CalculateX(x), CalculateY(y));
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

void Dialog::ReplaceSurfaceObject(const QPixmap& pixmap, const int x, const int y) {
	QTransform test;
	QGraphicsItem *item = scene->itemAt(CalculateX(x), CalculateY(y), test);
	QGraphicsPixmapItem *pixmap_item = dynamic_cast<QGraphicsPixmapItem*>(item);
	if (!pixmap_item)
		throw EvolvaException("Dialog::ReplaceSurfaceObject internal error");
	pixmap_item->setPixmap(pixmap);
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
	QScrollBar *sb = ui->log_textWindow->verticalScrollBar();
	ui->log_textWindow->insertPlainText(text);
	sb->setValue(sb->maximum());	
}


/** Method violates "program to interface" paradigm */
void Dialog::UpdateOverallStatistics() {
	std::shared_ptr<Field> field = Field::GetInstance();
	size_t carnies = field->stats_->carnivore_.Get();
	size_t herbis = field->stats_->herbivore_.Get();
	size_t omnis = field->stats_->omnivore_.Get();
	size_t disabled = field->stats_->disabled_.Get();
	size_t trees = field->stats_->tree_.Get();
	size_t flesh = field->stats_->flesh_.Get();
	size_t miscarries = field->stats_->miscarry_.Get();
	size_t fights = field->stats_->fight_.Get();
	size_t escapes = field->stats_->escape_.Get();
	ui->lineEdit_carnies->setText(QString::fromStdString(std::to_string(carnies)));
	ui->lineEdit_herbis->setText(QString::fromStdString(std::to_string(herbis)));
	ui->lineEdit_omnis->setText(QString::fromStdString(std::to_string(omnis)));
	ui->lineEdit_disabled->setText(QString::fromStdString(std::to_string(disabled)));
	ui->lineEdit_trees->setText(QString::fromStdString(std::to_string(trees)));
	ui->lineEdit_flesh->setText(QString::fromStdString(std::to_string(flesh)));
	ui->lineEdit_miscarries->setText(QString::fromStdString(std::to_string(miscarries)));
	ui->lineEdit_fights->setText(QString::fromStdString(std::to_string(fights)));
	ui->lineEdit_escapes->setText(QString::fromStdString(std::to_string(escapes)));
}


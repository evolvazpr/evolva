#include "dialog.hpp"
#include "ui_dialog.h"
#include "Field.hpp"
#include "EvolvaException.hpp"
#include "Unit.hpp"


Dialog* Dialog::dialog_ = nullptr;

/**
 * @brief ANIMATION_CLOCK static global variable. Parameter to setup freqency of animation (FPS).
 */
static const qreal ANIMATION_CLOCK = 1000/33;

/**
 * @brief Pixels per object
 */
static const uint PIXELS_PER_OBJECT = 25;


/**
 * @brief Constructor.
 * @param parent - parent for Qt API (no need to call delete thanks to it).
 */
Dialog::Dialog(QWidget *parent, const int width, const int height) :
	QDialog(parent), ui(new Ui::Dialog), width_(width), height_(height) { 
	Qt::WindowFlags flags = Qt::WindowTitleHint | Qt::WindowSystemMenuHint;
	flags |= Qt::WindowCloseButtonHint | Qt::WindowMinMaxButtonsHint;
	
	ui->setupUi(this);
	scene = new QGraphicsScene();
	ui->graphicsView->setScene(scene);
	scene->setSceneRect(0, 0, width_ * PIXELS_PER_OBJECT, height_ * PIXELS_PER_OBJECT);
	ui->graphicsView->centerOn(scene->sceneRect().center());
	ui->graphicsView->setCacheMode(QGraphicsView::CacheNone);
	ui->graphicsView->setRenderHint(QPainter::Antialiasing);
	ui->graphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
	ui->graphicsView->show();

	QDialog::setWindowFlags(flags);
	timer_.start(ANIMATION_CLOCK);		
}

/**
 * @brief Deconstructor.
 */
Dialog::~Dialog() {
	delete ui;
}


/**
 * @brief Singleton method to get pointer to object. 
 * @param parent - optional parameter used in object's first initalization. Used to set parent
 *		   of dialog.
 * @param width - width of field.
 * @param height - height of field.
 */
Dialog * Dialog::GetInstance(QWidget *parent, const int width, const int height) {
	if (dialog_ == nullptr) {
		if (width == 0 || height == 0)
			throw EvolvaException("Error: width or height of GUI field set to 0!");
		dialog_ = new Dialog(parent, width, height);
	}
	return dialog_;
}


/**
 * @brief pushButton action method.
 * Method called when "Kolejna tura" button was clicked.
 */
void Dialog::on_pushButton_clicked() {	
	emit NextLogicIteration();
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
	int x_pos = CalculateX(x);
	int y_pos = CalculateY(y);
	SpriteObject *sprite_object;
	std::string obj_type;
	QString sprite_path;
	sprite_object = new SpriteObject(scene->parent(), id, x_pos, y_pos, 
					path, sprite_cnt, PIXELS_PER_OBJECT);
	scene->addItem(sprite_object);
	
	QObject::connect(dynamic_cast<QObject *>(sprite_object), SIGNAL(AnimationFinished()), 
			this, SIGNAL(ClearMutex()));
	QObject::connect(dynamic_cast<QObject *>(sprite_object), SIGNAL(wasClicked(int, int)), this, 
			SIGNAL(SpriteObjectClicked(int, int)));
	QObject::connect(&timer_, SIGNAL(timeout()), dynamic_cast<QObject *>(sprite_object), 
			SLOT(animate()));
	emit ClearMutex();
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
			if(ptr->id() == id)
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
	SpriteObject *roundObject = SearchObject(id);
	if (!roundObject) {
		throw EvolvaException("Dialog::moveObject - object not found!\n");
	}
	roundObject->move(CalculateX(x), CalculateY(y));	
}

/**
 * @brief Move object method to specific field's coordinate.
 * @param object - shared_ptr of object that will be moved.
 * @param x - <b>real field's x coordinate</b> in which RoundObject will be placed.
 * @param y - <b>real field's y coordinate</b> in which RoundObject will be placed.
 */
void Dialog::MoveObjectTo(const uint id, const int x, const int y) {
	SpriteObject *roundObject = SearchObject(id);
	int x_old, y_old, dx, dy;
	if (!roundObject) {
		throw EvolvaException("Dialog::moveObjectTo - object not found!\n");
	}

	x_old = roundObject->x();
	y_old = roundObject->y();

	dx = CalculateX(x) - x_old;
	dy = CalculateY(y) - y_old;

	roundObject->move(dx, dy);
}

/**
 * @brief Remove (delete) specific SpriteObject.
 * @param object - shared_ptr to object which will be deleted from GUI.
 */
void Dialog::RemoveObject(const uint id) {
	SpriteObject *roundObject = SearchObject(id);

	if (!roundObject) 
		throw EvolvaException("Dialog::removeObject - object not found!\n");	

	scene->removeItem(roundObject);
	delete(roundObject);
	emit ClearMutex();
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
	
	pix_map = pix_map.scaled(PIXELS_PER_OBJECT, PIXELS_PER_OBJECT, 
				 Qt::IgnoreAspectRatio, 
				 Qt::SmoothTransformation);
	QGraphicsPixmapItem *rect = new QGraphicsPixmapItem(pix_map);
	rect->setOffset(CalculateX(x), CalculateY(y));
	scene->addItem(rect);
	emit ClearMutex();
}

/**
 * @brief Deletion of graphic surface.
 * @param x - x coordinate.
 * @param y - y coordinate.
 */
void Dialog::RemoveSurfaceObject(const int x, const int y)
{
	QTransform test;
	QGraphicsItem *item = scene->itemAt(CalculateX(x), CalculateY(y), test);
	if (!item)
		return;
	scene->removeItem(item);
	delete(item);
	emit ClearMutex();	
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



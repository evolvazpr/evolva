#include "dialog.hpp"
#include "ui_dialog.h"
#include "Field.hpp"
#include "Tui.hpp"
#include "CellObject.hpp"
#include "EvolvaException.hpp"
#include "RoundObject.hpp"
/**
 * @brief ANIMATION_CLOCK static global variable. Parameter to setup freqency of animation (FPS or whatever).
 */
static const qreal ANIMATION_CLOCK = 1000/33;

/**
 * @brief FIELD_SIZE temporary?
 */
static const uint FIELD_SIZE = 10;

/**
 * @brief Pixels per object
 */
static const uint PIXELS_PER_OBJECT = 50;

/**
 * @brief Constructor.
 * @param parent - parent for Qt API (no need to call delete thanks to it).
 */
Dialog::Dialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::Dialog), width_(FIELD_SIZE), height_(FIELD_SIZE) {
	QRect rect;
	ui->setupUi(this);
	scene = new QGraphicsScene();
	ui->graphicsView->setScene(scene);
	scene->setSceneRect(0, 0, FIELD_SIZE * PIXELS_PER_OBJECT, FIELD_SIZE * PIXELS_PER_OBJECT);
	ui->graphicsView->centerOn(scene->sceneRect().center());
	ui->graphicsView->setCacheMode(QGraphicsView::CacheNone);
	ui->graphicsView->setRenderHint(QPainter::Antialiasing);
	ui->graphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
	ui->graphicsView->show();
	animations_ = 0;
	timer.start(ANIMATION_CLOCK);
}

/**
 * @brief Deconstructor.
 */
Dialog::~Dialog() {
	delete ui;
}

void Dialog::on_pushButton_clicked() {          //what happens when we click the "Kolejna tura" button
	static Tui tui;
	std::shared_ptr<MovableObject> next;
	next = field->Next();
	if (next == nullptr)
		exit(0);
	tui.PrintField();
	field->f2();
}
/**
 * @brief Graphical x coordinate calculation.
 * @param x - field's x coordinate.
 * @return  - graphical x coordinate.
 */
int Dialog::calculateX(const int x) {
	return ((qreal)x) / ((qreal)width_ )* scene->width();
}

/**
 * @brief Graphical y coordinate calculation.
 * @param y - field's y coordinate.
 * @return  - graphical y coordinate.
 */
int Dialog::calculateY(const int y) {
	return ((qreal)y) / ((qreal)height_) * scene->height();
}

/**
 * @brief Graphical radius coordinate calculation.
 * @return
 */
uint Dialog::calculateRadius() {
	return  scene->height()/(qreal)height_;
}

/**
 * @brief RoundObject object creation.
 * @param id - object's id.
 * @param x - field's x coordinate of object.
 * @param y - field's y coordinate of object.
 */
void Dialog::CreateObject(std::shared_ptr<const CellObject> object, const int x, const int y) {
	int x_pos = calculateX(x);
	int y_pos = calculateY(y);
	int radius =  calculateRadius();
	RoundObject *roundObject;
	QColor color = Qt::white;
	if (object->GetType(CellObject::Type::MOVABLE)) {
		if (object->GetType(CellObject::Type::CARNIVORE)) 
			color = Qt::red;
		else if (object->GetType(CellObject::Type::HERBIVORE))
			color = Qt::darkGreen;
		else
			color = Qt::black;
	} else {
		if (object->GetType(CellObject::Type::PLANT))
			color = Qt::green;
		else 
			color = Qt::gray;
	}	
	roundObject = new RoundObject(object->GetId(), x_pos, y_pos, radius, scene, &timer, color);
	QObject::connect(dynamic_cast<QObject *>(roundObject), SIGNAL(AnimationFinished()), 
			 this, SLOT(AnimationFinished()));
}

/**
 * @brief RoundObject object search method.
 * @param id - object's id.
 * @return pointer to RoundObject. If there is RoundObject with passed id, nullptr is returned.
 */
RoundObject* Dialog::SearchObject(const uint id) {
	QList<QGraphicsItem *> obj_list = scene->items();
	RoundObject *ptr;
	for (auto &it : obj_list) {
		if (it->type() == QGraphicsEllipseItem::Type) {
			ptr = static_cast<RoundObject *>(it);
			if(ptr->id() == id)
				return ptr;
		}
	}
	return nullptr;
}

void Dialog::IncrementAnimations(RoundObject *roundObject){
	if (roundObject->IsMoving())
		animations_.fetchAndAddAcquire(0);
	else
		animations_.fetchAndAddAcquire(1);
}

/**
 * @brief Move object by relative coordinates (in other words - move with (x, y) steps somewhere).
 *
 * By relative coordinates it is meant to be in relation to present object's position.
 * @param object - shared_ptr to object which will be moved.
 * @param x - <b>realative field (not graphical) steps</b> in x direction to make.
 * @param y - <b>realative field (not graphical) steps</b> in y direction to make.
 */
void Dialog::MoveObject(std::shared_ptr<const CellObject> object, const int x, const int y) {
	RoundObject *roundObject = SearchObject(object->GetId());
	if (!roundObject)
		throw EvolvaException("Dialog::moveObject - object not found!\n");

	IncrementAnimations(roundObject);	

	roundObject->move(calculateX(x), calculateY(y));
}

/**
 * @brief Move object method to specific field's coordinate.
 * @param object - shared_ptr of object that will be moved.
 * @param x - <b>real field's x coordinate</b> in which RoundObject will be placed.
 * @param y - <b>real field's y coordinate</b> in which RoundObject will be placed.
 */
void Dialog::MoveObjectTo(std::shared_ptr<const CellObject> object, const int x, const int y) {
	RoundObject *roundObject = SearchObject(object->GetId());
	int x_old, y_old, dx, dy;
	if (!object)
		throw EvolvaException("Dialog::moveObjectTo - object not found!\n");

	IncrementAnimations(roundObject);

	x_old = roundObject->x();
	y_old = roundObject->y();

	dx = calculateX(x) - x_old;
	dy = calculateY(y) - y_old;
	
	roundObject->move(dx, dy);
}

/**
 * @brief Remove (delete) specific RoundObject.
 * @param object - shared_ptr to object which will be deleted from GUI.
 */
void Dialog::RemoveObject(std::shared_ptr<const CellObject> object) {
	RoundObject *roundObject = SearchObject(object->GetId());
	if (!roundObject) 
		throw EvolvaException("Dialog::removeObject - object not found!\n");	
	to_remove_.push_back(roundObject);
	if (!animations_.fetchAndAddAcquire(0))
		ClearField();
}

void Dialog::ClearField() {
	static QMutex mutex; //TODO RAII!!
	mutex.lock();
	for(auto &it : to_remove_) {
		scene->removeItem(it);
		delete(it);
	}
	to_remove_.clear();
	mutex.unlock();
}

void Dialog::AnimationFinished() {
	if (animations_.fetchAndAddAcquire(0))
		animations_.fetchAndAddAcquire(-1);
	if (!animations_.fetchAndAddAcquire(0))
		ClearField();
}

void Dialog::CreateGroundObject(const Dialog::Ground ground_type, const int x, const int y) {
	QString file;
	switch (ground_type) {
	case Dialog::Ground::GRASS :
		file = "../sprites/grass.png";
		break;
	case Dialog::Ground::SAND :
		file = "../sprites/sand.png";
		break;
	case Dialog::Ground::WATER :
		file = "../sprites/water.png";
		break;
	case Dialog::Ground::SOIL :
		file = "../sprites/soil.png";
		break;
	default:
		file = "";
		break;	
	}
	QPixmap pix_map(file);
	pix_map = pix_map.scaled(PIXELS_PER_OBJECT, PIXELS_PER_OBJECT, 
				 Qt::KeepAspectRatio, 
				 Qt::SmoothTransformation);
	if (pix_map.isNull())
		throw EvolvaException("Ground sprite could not have been loaded. Aborting program.\n");
	QGraphicsPixmapItem *rect = new QGraphicsPixmapItem(pix_map);
	rect->setOffset(calculateX(x), calculateY(y));
	scene->addItem(rect);
}

void Dialog::RemoveGroundObject(const int x, const int y)
{
	QTransform test;
	QGraphicsItem *item = scene->itemAt(calculateX(x), calculateY(y), test);
	if (!item)
		return;
	scene->removeItem(item);
	delete(item);	
}

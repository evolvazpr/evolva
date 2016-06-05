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
	QDialog(parent), settings_("gui.xml"),
	ui(new Ui::Dialog), width_(width), height_(height) { 
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
	animations_ = 0;

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
 * It calls Field::Next() method and other method for debug.
 */
void Dialog::on_pushButton_clicked() {  
	field->Next();
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
void Dialog::CreateObject(std::shared_ptr<const CellObject> object, const int x, const int y) {
	int x_pos = CalculateX(x);
	int y_pos = CalculateY(y);
	int sprite_cnt = 1;
//	QMutexLocker lock(&gui_mutex_);
//	gui_mutex_.lock();
//	gui_mutex_.unlock();
	SpriteObject *sprite_object;
	std::string obj_type;
	QString sprite_path;
	obj_type = GetTypeName(object);
	sprite_path = QString::fromStdString(settings_[obj_type][std::string("path")]);
	sprite_cnt = settings_[obj_type][std::string("sprite_cnt")];
	sprite_object = new SpriteObject(&gui_mutex_, scene->parent(), object->GetId(), x_pos, y_pos, 
					sprite_path, sprite_cnt, PIXELS_PER_OBJECT);
	scene->addItem(sprite_object);
	
	QObject::connect(dynamic_cast<QObject *>(sprite_object), SIGNAL(AnimationFinished()), 
			this, SLOT(AnimationFinished()));
	QObject::connect(dynamic_cast<QObject *>(sprite_object), SIGNAL(wasClicked(int, int)), this, 
			SLOT(SpriteObjectClicked(int, int)));
	QObject::connect(&timer_, SIGNAL(timeout()), dynamic_cast<QObject *>(sprite_object), 
			SLOT(animate()));
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
			ptr = dynamic_cast<SpriteObject *>(it);
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
void Dialog::MoveObject(std::shared_ptr<const CellObject> object, const int x, const int y) {
	SpriteObject *roundObject = SearchObject(object->GetId());
	if (!roundObject)
		throw EvolvaException("Dialog::moveObject - object not found!\n");

	IncrementAnimations(roundObject);	

	roundObject->move(CalculateX(x), CalculateY(y));	
}

/**
 * @brief Move object method to specific field's coordinate.
 * @param object - shared_ptr of object that will be moved.
 * @param x - <b>real field's x coordinate</b> in which RoundObject will be placed.
 * @param y - <b>real field's y coordinate</b> in which RoundObject will be placed.
 */
void Dialog::MoveObjectTo(std::shared_ptr<const CellObject> object, const int x, const int y) {
	SpriteObject *roundObject = SearchObject(object->GetId());
	int x_old, y_old, dx, dy;
	if (!object)
		throw EvolvaException("Dialog::moveObjectTo - object not found!\n");

	IncrementAnimations(roundObject);

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
void Dialog::RemoveObject(std::shared_ptr<const CellObject> object) {
	SpriteObject *roundObject = SearchObject(object->GetId());
	QMutexLocker lock(&gui_mutex_);

	if (!roundObject) 
		throw EvolvaException("Dialog::removeObject - object not found!\n");	
	to_remove_.push_back(roundObject);

	if (!animations_.fetchAndAddAcquire(0))
		UpdateField();
}

/**
 * @brief ClearField from object, that should be removed, but weren't, because animation
 * was on-going.
 */
void Dialog::UpdateField() {
	for(auto &it : to_remove_) {
		scene->removeItem(it);
		delete(it);
	}
	to_remove_.clear();
}

/**
 * @brief Incrementation of animating object counter.
 *
 * This was made to eliminate bug which caused to early object remove from QGraphicsScene.
 * Without it, firstly object A was removed, then object B (which is a cause of object remove)
 * was moved to object A. Now order is reached - firstly object B is moved to A, secondly object
 * A is removed from screen.
 *
 * @param roundObject - pointer to SpriteObject to get information wether this object is moving.
 */ 
void Dialog::IncrementAnimations(SpriteObject *roundObject){
	if (roundObject->IsMoving())
		animations_.fetchAndAddAcquire(0);
	else
		animations_.fetchAndAddAcquire(1);
}


/**
 * @brief Decrementation of animating objects counter.
 *
 * Reason for this counter is described in Dialog::IncrementAnimations method description.
 */
void Dialog::AnimationFinished() {
	if (animations_.fetchAndAddAcquire(0))
		animations_.fetchAndAddAcquire(-1);
	if (!animations_.fetchAndAddAcquire(0)) {
		UpdateField();
	}
}


/**
 * @brief Creation of graphic surface.
 *	
 * @param surface_type - Type of surface to create.
 * @param x - x coordinate.
 * @param y - y coordinate.
 */
void Dialog::CreateSurfaceObject(const FieldCell::Ground surface, const int x, const int y) {
	QString file;
	std::string xml_cmd;

	switch (surface) {
	case FieldCell::Ground::GRASS :
		xml_cmd = "grass";
		break;
	case FieldCell::Ground::GROUND :
		xml_cmd = "soil";
		break;
	default:
		throw EvolvaException("Wrong surface_type in CreateSurfaceObject!");
		break;	
	}
	
	QPixmap pix_map(QString::fromStdString(settings_[xml_cmd]["path"]));
	if (pix_map.isNull())
		throw EvolvaException("Sprite \"" + xml_cmd + "\" could not have been loaded. Aborting program.\nCheck if gui.xml is correct.");
	
	pix_map = pix_map.scaled(PIXELS_PER_OBJECT, PIXELS_PER_OBJECT, 
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
void Dialog::RemoveSurfaceObject(const int x, const int y)
{//TODO QTransform test??
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
void Dialog::AppendTextToLog(const std::string text) {
	ui->log_textWindow->append(QString::fromStdString(text));
}

/**
 * @brief stream operator overload.
 * Used to append text to log window.
 * @param s - std::string to append.
 */
Dialog& operator <<(Dialog& dialog, const std::string s) {	
	dialog.ui->log_textWindow->insertPlainText(QString::fromStdString(s));
	return dialog;
}

/**
 * @brief stream operator overload.
 * Used to append text to log window.
 * @param s - char pointer to append.
 */
Dialog& operator <<(Dialog& dialog, const char* s) {	
	dialog.ui->log_textWindow->insertPlainText(s);
	return dialog;
}


/**
 * @brief method used when sprite object was clicked (so stats window will be updated).
 * This method creates content of stats window.
 * param cell - Field's cell in which object, represented by clicked sprite, resides.
 */
boost::format Dialog::CreateStatistics(std::shared_ptr<FieldCell> cell) {
	std::shared_ptr<CellObject> object;
	std::shared_ptr<Unit> unit;
	std::shared_ptr<Plant> plant;
	boost::format form;
	object = cell->GetObject();
		
	if (object.use_count() == 0)
		throw EvolvaException("Dialog::CreateStatistics, cell->IsEmpty() failed.");

	form = boost::format("Id: %1%\nx: %2%\ny: %3%\nEnergy: %4%\nFatigue: %5%\nType: %6%");
	form % object->GetId();
	form % object->GetX();
	form % object->GetY();
	
	if (object->GetType(CellObject::Type::MOVABLE)) {
		unit = std::dynamic_pointer_cast<Unit>(object); 
		/* Above line proofs wrong interface of CellObject.
		 * I should write there GetEnergy() and GetFatigue() virtual methods also.
		 * Without it, I must cast.
		 */
		if (unit.use_count() == 0)
			throw EvolvaException("Dialog::CreateStatistics, object->GetType(MOVABLE) failed");
		form % unit->GetEnergy();	
		form % unit->GetFatigue();
	} else {

		if(object->GetType(CellObject::Type::PLANT)) {
			plant = std::dynamic_pointer_cast<Plant>(object);
			if (plant.use_count() == 0)
				throw EvolvaException("Dialog::CreateStatistics, object->GetType(PLANT) failed.");		
			form % plant->GetEnergy();
			form % 0;
		} else {
			form % 0;
			form % 0;
		}	
	}
	form % GetTypeName(object);	
	return form;
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
void Dialog::SpriteObjectClicked(int x, int y) {
	std::shared_ptr<Field> field = Field::GetInstance();
	std::shared_ptr<FieldCell> cell = field->GetCell(x, y);
	boost::format form;
	if(cell->IsEmpty()) {
		form = boost::format("Cell is empty.");
	} else {
		form = CreateStatistics(cell);
	}

	ui->stats_textWindow->setPlainText(QString::fromStdString(form.str()));		
}

std::string Dialog::GetTypeName(std::shared_ptr<const CellObject> object) {
	std::string obj_type;
	if (object->GetType(CellObject::Type::CARNIVORE)) { 
		if (object->GetType(CellObject::Type::FLESH))
			obj_type = "carnivore_dead";
		else
			obj_type = "carnivore";
	} else if (object->GetType(CellObject::Type::HERBIVORE)) {
		if (object->GetType(CellObject::Type::FLESH))
			obj_type = "herbivore_dead";
		else
			obj_type = "herbivore";
	} else if (object->GetType(CellObject::Type::PLANT)) {
		obj_type = "tree";
	} else {
		obj_type = "stone";
	}	
	return obj_type;
}

#include "Field.hpp"

std::shared_ptr<Field> Field::instance_ = std::shared_ptr<Field>(nullptr);

Field::ListElement::ListElement(std::shared_ptr<CellObject> object, int x, int y) :
	object_(object), x_physical_(x), y_physical_(y){};

Field::Field(const int x, const int y) : x_max_(x), y_max_(y), cells_(boost::extents[x][y]) {

	for (int x_i = 0; x_i < x; x_i++) {

		for (int y_i = 0; y_i < y; y_i++) {

			cells_[x_i][y_i] = std::shared_ptr<FieldCell>(new FieldCell());

		}
	}	
}	

Field & Field::getInstance(const int x, const int y) {

	if (!instance_) {

				instance_ = std::shared_ptr<Field>(new Field(x, y));

			}

			return *instance_; 
}

std::shared_ptr<FieldCell> Field::getCell(int x, int y) {

	if (x >= x_max_)
		return nullptr;

	else if (y >= x_max_)
		return nullptr;

	else
		return cells_[x][y];
}

std::shared_ptr<FieldCell> Field::getCellWithRelativeCoords(std::shared_ptr<CellObject> object, int x_relative,
																														int y_relative){
	for (auto it: object_list_) {

		if (object == it->object_)
			return getCell(it->x_physical_ + x_relative, it->y_physical_ + y_relative);	
	}
	return nullptr;
}

void Field::calculateNewPositionOfObject(std::shared_ptr<ListElement> element, int x_relative, int y_relative){
	element->x_physical_ += x_relative;
	element->x_physical_ %= x_max_;

	element->y_physical_ += y_relative;
	element->y_physical_ %= y_max_;
}

bool Field::moveObjectWithRelativeStep(std::shared_ptr<CellObject> object, int x_relative, int y_relative) {
	bool ret;
	std::shared_ptr<FieldCell> cell_old, cell_new;
	
	if (!object->isMovable())
		return false;
	
	for (auto it: object_list_) {
	
		if (object == it->object_) {
	
			cell_old = getCell(it->x_physical_, it->y_physical_);
			cell_new = getCell(it->x_physical_ + x_relative, it->y_physical_ + y_relative);
			ret = cell_new->insertObject(object);
	
			if (!ret) {
	
				return false;
	
			}	else {

				cell_old->removeObject();
				calculateNewPositionOfObject(it, x_relative, y_relative);
				return true;

			}
		}
	}
	return false;
}

bool Field::addObject(std::shared_ptr<CellObject> object, int x, int y) {
			bool ret = false;
			std::shared_ptr<FieldCell> cell = getCell(x, y);

			if (cell == nullptr)
				return false;
		
			ret = cell->insertObject(object);
			if (!ret)
				return false;
		
			object_list_.push_back(std::shared_ptr<ListElement>(new ListElement(object, x, y)));

			return true;
}

bool Field::removeObject(std::shared_ptr<CellObject> object){
	std::shared_ptr<FieldCell> cell;
	for (auto it = std::begin(object_list_); it < std::end(object_list_); ++it) {	
		if (object == (*it)->object_) {
			cell = getCell((*it)->x_physical_, (*it)->x_physical_);
			cell->removeObject();
			object_list_.erase(it);
			return true;
		}	
	}	
	return false;
}

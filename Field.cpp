#include "Field.hpp"

std::shared_ptr<Field> Field::instance_ = std::shared_ptr<Field>(nullptr);

Field::Field(const int x, const int y) : x_max_(x), y_max_(y) {
	for (int i = 0; i < x * y; i++) {
		cells_.push_back(std::shared_ptr<FieldCell>(new FieldCell()));
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
		return  cells_[x * y_max_ + y];
}

std::shared_ptr<FieldCell> Field::getCellWithRelativeCoords(std::shared_ptr<CellObject> object, int x_relative, int y_relative){
	for (auto it: object_list_) {
		if (object == it->object_)
			return getCell(it->x_physical_ + x_relative, it->y_physical_ + y_relative);	
	}
	return nullptr;
}

bool Field::moveObjectWithRelativeStep(std::shared_ptr<CellObject> object, int x_relative, int y_relative) {
	bool ret;
	std::shared_ptr<FieldCell> cell_old, cell_new;
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

void Field::calculateNewPositionOfObject(std::shared_ptr<listElement> element, int x_relative, int y_relative){
	element->x_physical_ += x_relative;
	element->x_physical_ %= x_max_;

	element->y_physical_ += y_relative;
	element->y_physical_ %= y_max_;
}

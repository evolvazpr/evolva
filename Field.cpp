#include "Field.hpp"

std::shared_ptr<Field> Field::instance_ = std::shared_ptr<Field>(nullptr);

Field::ListElement::ListElement(std::shared_ptr<CellObject> object, int x, int y) 
	: object_(object), x_physical_(x), y_physical_(y) {};

Field::Field(const int x, const int y) : x_max_(x), y_max_(y), cells_(boost::extents[x][y]) {
	for (int x_i = 0; x_i < x; x_i++) {
		for (int y_i = 0; y_i < y; y_i++) {
			cells_[x_i][y_i] = std::make_shared<FieldCell>();
		}
	}	
}	

Field& Field::GetInstance(const int x, const int y) {
	if (!instance_) {
			//TODO: replace std;:shared_ptr with std::make_shared
				instance_ = std::shared_ptr<Field>(new Field(x, y));
			}
			return *instance_; 
}

std::weak_ptr<FieldCell> Field::GetCell(int x, int y) {
	return std::weak_ptr<FieldCell>(cells_[x][y]);
}

std::weak_ptr<FieldCell> Field::GetCellWithRelativeCoords(std::shared_ptr<CellObject> object, int x_relative,
																														int y_relative) {
	std::weak_ptr<FieldCell> ret;
	for (auto it: object_list_) {
		if (object == it->object_) 
			ret = GetCell(it->x_physical_ + x_relative, it->y_physical_ + y_relative);	
			break;
	}
	return ret;
}

void Field::CalculateNewPositionOfObject(std::shared_ptr<ListElement> element, int x_relative, int y_relative) {
	element->x_physical_ += x_relative;
	element->x_physical_ %= x_max_;

	element->y_physical_ += y_relative;
	element->y_physical_ %= y_max_;
}

bool Field::MoveObjectWithRelativeStep(std::shared_ptr<CellObject> object, int x_relative, int y_relative) {
	bool ret;
	std::shared_ptr<FieldCell> cell_old, cell_new;
	if (!object->isMovable()) return false;
	for (auto it: object_list_) {
		if (object == it->object_) {
			cell_old = std::shared_ptr<FieldCell>(GetCell(it->x_physical_, it->y_physical_));
			cell_new = std::shared_ptr<FieldCell>(GetCell(it->x_physical_ + x_relative, 
																										 it->y_physical_ + y_relative));
			ret = cell_new->insertObject(object);
			if (!ret) {
				return false;
			}	else {
				cell_old->removeObject();
				CalculateNewPositionOfObject(it, x_relative, y_relative);
				return true;	
			}
		}
	}
	return false;
}

bool Field::AddObject(std::shared_ptr<CellObject> object, int x, int y) {
	bool ret = false;
	std::shared_ptr<FieldCell> cell = std::shared_ptr<FieldCell>(GetCell(x, y));
	if (cell == nullptr) return false;
	ret = cell->insertObject(object);
	if (!ret) return false;
	object_list_.push_back(std::shared_ptr<ListElement>(new ListElement(object, x, y)));
	return true;
}

bool Field::RemoveObject(std::shared_ptr<CellObject> object) {
	std::shared_ptr<FieldCell> cell;
	for (auto it = std::begin(object_list_); it < std::end(object_list_); ++it) {		
		if (object == (*it)->object_) {
			cell = std::shared_ptr<FieldCell>(GetCell((*it)->x_physical_, (*it)->x_physical_));
			cell->removeObject();
			object_list_.erase(it);
			return true;
		}	
	}	
	return false;
}

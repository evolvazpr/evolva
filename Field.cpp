#include "Field.hpp"

Field * Field::instance_ = nullptr;

Field::Field(const int x, const int y) : x_max_(x), y_max_(y), cells_(new FieldCell[x * y]){} 

Field & Field::getInstance(const int x, const int y) {
			if (!instance_) {
				instance_ = new Field(x, y);
			}
			return *instance_; 
}

FieldCell & Field::getCell(int x, int y) {
	return cells_[x * y_max_ + y];
}
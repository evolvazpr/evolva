#include "FieldCell.hpp"

GroundType FieldCell::getGroundType() {
	return ground_type_;
}

bool FieldCell::insertObject(std::shared_ptr<CellObject> obj) {
	if (object_.lock()) return false; 
	object_ = obj;
	return true;
}

void FieldCell::removeObject() {
	object_.reset();
}

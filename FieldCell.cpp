#include "FieldCell.hpp"

bool FieldCell::InsertObject(std::shared_ptr<CellObject> obj) {
	if (object_.lock()) return false; 
	object_ = obj;
	return true;
}

void FieldCell::RemoveObject() {
	object_.reset();
}

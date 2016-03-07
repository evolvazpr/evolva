#include "FieldCell.hpp"

bool FieldCell::InsertObject(std::shared_ptr<CellObject> obj) {
	if (!IsEmpty()) return false; 
	object_ = obj;
	return true;
}

void FieldCell::RemoveObject() {
	object_.reset();
}

bool FieldCell::IsEmpty() {
	if (object_.lock()) return false;
	else return true;
}

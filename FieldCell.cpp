#include "FieldCell.hpp"
#include "CellObject.hpp"
#include "Unit.hpp"
#include "Field.hpp"
#include <random>

FieldCell::FieldCell(const size_t x, const size_t y) {
	object_ = nullptr;
	x_ = x;
	y_ = y;
}

FieldCell::~FieldCell() {
}

std::shared_ptr<Unit> FieldCell::GetUnit() {
	return std::static_pointer_cast<Unit>(GetObject());
}

bool FieldCell::SetObject(std::shared_ptr<CellObject> object) {
	// If cell is empty and new object is nullptr - do nothing
	if (static_cast<bool>(object)) {
		if (IsEmpty()) {
			object_ = object;
            object_->cell_ = shared_from_this();
            return true;
		}
		else {
			// Set new object parent cell to this cell
			object_->cell_ = object->cell_;
			// Set new object
			object_ = object;
			// Return output
			return true;
		}
	}
	else {
		object_.reset();
		return true;
	}
}

std::shared_ptr<NonMovableObject> FieldCell::GetNmo() {
	return std::static_pointer_cast<NonMovableObject>(object_);
}

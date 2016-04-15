#include "FieldCell.hpp"
#include "CellObject.hpp"
#include "Unit.hpp"

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

std::shared_ptr<CellObject> FieldCell::SetObject(std::shared_ptr<CellObject> object) {
	// If cell is empty and new object is nullptr - do nothing
	if (static_cast<bool>(object)) {
		if (IsEmpty()) {
			object_ = object;
            object_->cell_ = shared_from_this();
            return nullptr;
		}
		else {
			// Remove current object (move to output)
			std::shared_ptr<CellObject> output(std::move(object_));
			// Set new object
			object_ = object;
			// Set new object parent cell to this cell
			object_->cell_ = output->cell_;
			// Reset output object parent cell
			output->cell_.reset();
			// Return output
			return std::move(output);
		}
	}
	else {
		std::shared_ptr<CellObject> output(std::move(object_));
		object_.reset();
		return std::move(output);
	}
}

std::shared_ptr<NonMovableObject> FieldCell::GetNmo() {
	return std::static_pointer_cast<NonMovableObject>(object_);
}

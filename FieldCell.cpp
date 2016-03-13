#include "FieldCell.hpp"

FieldCell::FieldCell() {
	object_ = nullptr;
}

FieldCell::~FieldCell() {
}

std::shared_ptr<CellObject> FieldCell::SetObject(std::shared_ptr<CellObject> object) {
	std::shared_ptr<CellObject> output(std::move(object_));
	object_ = object;
	return std::move(output);
}

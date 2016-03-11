#include "FieldCell.hpp"

//implement Field()

//implement ~Field

std::shared_ptr<CellObject> FieldCell::SetObject(std::shared_ptr<CellObject> object) {
	std::shared_ptr<CellObject> output(std::move(object_));
	object_ = object;
	return std::move(output);
}

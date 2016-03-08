#include "FieldCell.hpp"

//implement Field()

//implement ~Field

std::shared_ptr<CellObject> SetObject(std::shared_ptr<CellObject> object) {
	std::shared_ptr<CellObject> output(std::move(object_));
	if (!object.IsEmpty) object_ = object;
	return std::move(output);
}

#include "FieldCell.hpp"

GroundType FieldCell::getGroundType() {
	std::cout << "ground_type_ : " << ground_type_ << std::endl;
	return ground_type_;
}

bool FieldCell::insertObject(std::shared_ptr<CellObject> obj){
	if (object_.lock()) {
		return false;
	} 
	object_ = obj;
	return true;
}

void FieldCell::removeObject(void){
	object_.reset();
}

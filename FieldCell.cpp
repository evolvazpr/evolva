#include "FieldCell.hpp"

GroundType FieldCell::getGroundType() {

	std::cout << "ground_type_ : " << ground_type_ << std::endl;
	return ground_type_;
}

bool FieldCell::insertObject(std::shared_ptr<Object> obj){
	if (object_.lock())
		return true;
		object_ = obj;
		return false;
}

void FieldCell::removeObject(void){
	object_.reset();
}

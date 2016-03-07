#include "CellObject.hpp"

RealCoordinates::~RealCoordinates(){}
CellObject::~CellObject(){}
MovableObject::~MovableObject(){}
NonMovableObject::~NonMovableObject(){}

bool MovableObject::MoveWithRelativeStep(int x_relative, int y_relative) {
	std::pair<int, int>initial_real_coordinates(get_real_x(), get_real_y());
	std::pair<int, int>final_relative_cordinates(x_relative, y_relative);
	std::pair<int, int>final_real_coordinates(x_relative, y_relative);
	Field& field = Field::GetInstance();
	bool ret = field.MoveObject(std::shared_ptr<CellObject>(this), initial_real_coordinates,
															final_relative_coordinates, final_real_coordinates);
	if(!ret)
		return false;
	set_real_coordinates(final_real_coordinates);
	return true;
}

#include "Field.hpp"

std::shared_ptr<Field> Field::instance_ = std::shared_ptr<Field>(nullptr);

Field::Field(const int x, const int y) : x_max_(x), y_max_(y), cells_(boost::extents[x][y]) {
	for (int x_i = 0; x_i < x; x_i++) {
		for (int y_i = 0; y_i < y; y_i++) {
			cells_[x_i][y_i] = std::make_shared<FieldCell>();
		}
	}	
}	

std::shared_ptr<Field> Field::GetInstance(const int x, const int y) {
	if (!instance_) {
			//TODO: replace std;:shared_ptr with std::make_shared
				instance_ = std::shared_ptr<Field>(new Field(x, y));
			}
			return instance_; 
}

std::weak_ptr<FieldCell> Field::GetCell(int x, int y) {
	return std::weak_ptr<FieldCell>(cells_[x][y]);
}

bool Field::InsertObject(std::shared_ptr<MovableObject> object, int x, int y) {
	std::shared_ptr<FieldCell> cell = std::shared_ptr<FieldCell>(GetCell(x, y));
	bool ret = cell->InsertObject(std::shared_ptr<CellObject>(object));
	if (ret == false)
		return false;
	movable_objects_.insert(std::make_pair(object, std::make_pair(x, y)));
	return true;
}

bool Field::InsertObject(std::shared_ptr<NonMovableObject> object, int x, int y) {
	std::shared_ptr<FieldCell> cell = std::shared_ptr<FieldCell>(GetCell(x, y));
	bool ret = cell->InsertObject(std::shared_ptr<CellObject>(object));
	if (ret == false)
		return false;
	non_movable_objects_.push_back(object);
	return true;
}

bool Field::MoveObject(std::shared_ptr<MovableObject> object, int x_steps, int y_steps) {
	std::shared_ptr<FieldCell> initial_cell, final_cell;
	std::pair<int, int> initial_real_coordinates = movable_objects_[object];
	std::pair<int, int> final_real_coordinates = std::make_pair(initial_real_coordinates.first + x_steps,
																															initial_real_coordinates.second + y_steps);
	final_real_coordinates.first %= x_max_;
	final_real_coordinates.second %= y_max_;
	
	initial_cell = std::shared_ptr<FieldCell>(GetCell(initial_real_coordinates.first,
																						initial_real_coordinates.second));
	final_cell = std::shared_ptr<FieldCell>(GetCell(final_real_coordinates.first,
																					final_real_coordinates.second));
	if (!final_cell->IsEmpty()) return false;

	final_cell->InsertObject(std::dynamic_pointer_cast<CellObject>(object));
	movable_objects_[object] = final_real_coordinates;
	
	initial_cell->RemoveObject();

	//?TODO? Call GUI-update method

	return true;
}

void Field::IterateOverMovableObjects() {
	std::shared_ptr<FieldCell> cell;
	std::shared_ptr<CellObject> object;
	for(auto& it : movable_objects_) {	
		cell = std::shared_ptr<FieldCell>(GetCell(it.second.first, it.second.second));
		object = std::shared_ptr<CellObject>(cell->CopyObject());
		std::cout << object->GetId() << std::endl;
		if (it.first->GetId() != object->GetId())
			std::cout << "ERROR!\n";
	}
}

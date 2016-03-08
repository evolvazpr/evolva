#include "Field.hpp"

std::shared_ptr<Field> Field::instance_ = std::shared_ptr<Field>(nullptr);

Field::Field(const size_t x, const size_t y) : x_max_(x), y_max_(y), cells_(boost::extents[x][y]) {
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

bool Field::InsertObject(std::shared_ptr<CellObject> object, const size_t x, const size_t y) {
	std::shared_ptr<FieldCell> cell = GetCell(x, y);
	if (cell->IsEmpty()){
		cell->SetObject(object);
		return true;
	}
	else return false;
}

bool Field::InsertObject(std::shared_ptr<MovableObject> object, const size_t x, const size_t y) {
	bool insertion = InsertObject(static_cast<std::shared_ptr<CellObject>>(object), x, y);
	if (insertion){
		//TODO: add to queue WITH SORTING
	}
	else return false;
}

bool Field::InsertObject(std::shared_ptr<NonMovableObject> object, const size_t x, const size_t y) {
	bool insertion = InsertObject(static_cast<std::shared_ptr<CellObject>>(object), x, y);
	if (insertion){
		//TODO: add to vector
	}
	else return false;
}

bool Field::MoveObjectTo(std::shared_ptr<MovableObject> object, const size_t x, const size_t y) {
	std::shared_ptr<FieldCell> source_cell = GetCell(object->x_, object->y);
	if (source_cell != object) {
		//TODO: throw -> serious memory problem
	}
	if (!IsCorrect(x, y) return false;
	std::shared_ptr<FieldCell> cell = GetCell(x, y);
	if (!cell->IsEmpty()) return false;
	original_cell->RemoveObject();
	cell->SetObject(object);
	object->x_ = x;
	object->y_ = y;
	//TODO: Gui move
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

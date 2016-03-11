#include "Field.hpp"
#include "EvolvaException.hpp"

std::shared_ptr<Field> Field::instance_ = nullptr;

Field::Field(const size_t x, const size_t y) : width_(x), height_(y), cells_(boost::extents[x][y]) {
	for (size_t i = 0; i < x; i++) {
		for (size_t j = 0; j < y; j++) {
			cells_[i][j] = std::make_shared<FieldCell>();
		}
	}	
}	

std::shared_ptr<Field> Field::GetInstance(const size_t x, const size_t y) {
	if (!instance_) {
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
		moveable_objects_.push_back(object);
		return true;
	}
	else return false;
}

bool Field::InsertObject(std::shared_ptr<NonMovableObject> object, const size_t x, const size_t y) {
	bool insertion = InsertObject(static_cast<std::shared_ptr<CellObject>>(object), x, y);
	if (insertion){
		non_movable_objects_.push_back(object);
		return true;
	}
	else return false;
}

bool Field::MoveObjectTo(std::shared_ptr<MovableObject> object, const size_t x, const size_t y) {
	std::shared_ptr<FieldCell> source_cell = GetCell(object->x_, object->y_);
	if (source_cell->object_ != object) {
	throw EvolvaException("Serious memory problem.");
	}
	if (!IsCorrect(x, y)) return false;
	std::shared_ptr<FieldCell> cell = GetCell(x, y);
	if (!cell->IsEmpty()) return false;
	source_cell->RemoveObject();
	cell->SetObject(object);
	object->x_ = x;
	object->y_ = y;
	//TODO: Gui move
	return true;
}

bool Field::IsCorrect(const size_t x, const size_t y) const { return (x < width_ && y < height_); }
std::shared_ptr<FieldCell> Field::GetCell(const size_t x, const size_t y) { return cells_[x][y]; }
bool Field::MoveObject(std::shared_ptr<MovableObject> object, const long x_steps, const long y_steps)  { return MoveObjectTo(object, object->x_ + x_steps, object->y_ + y_steps); } 

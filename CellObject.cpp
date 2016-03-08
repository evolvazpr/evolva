#include "CellObject.hpp"

CellObject::~CellObject() {
}

MovableObject::~MovableObject() {
}

NonMovableObject::~NonMovableObject() {
}

CellObject::CellObject(const size_t id) : id_(id) {
}

MovableObject::MovableObject(const size_t id, const size_t move_priority) : CellObject(id), move_priority_(move_priority) {
}

NonMovableObject::NonMovableObject(const size_t id) : CellObject(id) {
}

bool MovableObject::MoveWithRelativeStep(int x, int y) {
	std::shared_ptr<Field> field = Field::GetInstance();
	return field->MoveObject(std::static_pointer_cast<MovableObject>(shared_from_this()), x, y);
}

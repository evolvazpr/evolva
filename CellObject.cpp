#include "Field.hpp"
#include "CellObject.hpp"

CellObject::~CellObject() {
}

MovableObject::~MovableObject() {
}

NonMovableObject::~NonMovableObject() {
}

CellObject::CellObject() : id_(field->GetFfid()) {
}

CellObject::CellObject(const size_t id) : id_(id) {
}

MovableObject::MovableObject() : CellObject() {
}

NonMovableObject::NonMovableObject() : CellObject() {
}

double MovableObject::GetMovePriority() const {
	return 0.0;
}

bool MovableObject::MoveWithRelativeStep(int x, int y) {
	std::shared_ptr<Field> field = Field::GetInstance();
	return field->MoveObject(std::static_pointer_cast<MovableObject>(shared_from_this()), x, y);
}

NonPlant::NonPlant() {
}

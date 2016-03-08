#include "CellObject.hpp"

CellObject::~CellObject(){}
MovableObject::~MovableObject(){}
NonMovableObject::~NonMovableObject(){}

CellObject::CellObject(int id) : id_(id) {}
MovableObject::MovableObject(int id, int move_priority) : CellObject(id), move_priority_(move_priority) {}
NonMovableObject::NonMovableObject(int id) : CellObject(id) {}

int CellObject::GetId() { return id_; }

bool MovableObject::MoveWithRelativeStep(int x, int y) {
	std::shared_ptr<Field> field = Field::GetInstance();
	return field->MoveObject(std::static_pointer_cast<MovableObject>(shared_from_this()), x, y);
}

#include "Field.hpp"
#include "CellObject.hpp"

// CellObject

CellObject::CellObject() : id_(field->GetFfid()) {
}

CellObject::CellObject(const size_t id) : id_(id) {
}

CellObject::~CellObject() {
}

bool CellObject::GetType(const CellObject::Type bit) const {
	return types_[static_cast<unsigned char>(bit)];
}

void CellObject::SetType(const CellObject::Type bit, const bool value) {
	types_[static_cast<unsigned char>(bit)] = value;
}

// MovableObject

MovableObject::MovableObject() : CellObject() {
	SetType(Type::MOVABLE, true);
}

MovableObject::~MovableObject() {
}

double MovableObject::GetMovePriority() const {
	return 0.0;
}

bool MovableObject::MoveWithRelativeStep(int x, int y) {
	std::shared_ptr<Field> field = Field::GetInstance();
	return field->MoveObject(std::static_pointer_cast<MovableObject>(shared_from_this()), x, y);
}

// NonMovableObject

NonMovableObject::NonMovableObject() : CellObject() {
	SetType(Type::MOVABLE, false);
}

NonMovableObject::~NonMovableObject() {
}

// NonPlant


NonPlant::NonPlant() : NonMovableObject() {
	SetType(Type::PLANT, false);
}

// Creature

Creature::Creature() : MovableObject() {
	SetType(Type::CREATURE, true);
}

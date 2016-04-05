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

bool MovableObject::Move(const int x, const int y, const bool trim) {
	return field->MoveObject(std::static_pointer_cast<MovableObject>(shared_from_this()), x, y, trim);
}

// NonMovableObject

NonMovableObject::NonMovableObject() : CellObject() {
	SetType(Type::MOVABLE, false);
}

NonMovableObject::~NonMovableObject() {
}


// Plant

Plant::Plant() : NonMovableObject() {
	SetType(Type::PLANT, true);
}

Plant::~Plant() {
}

bool Plant::Eat(double energy) {
	if (energy <= energy_) {
		energy_ -= energy;
		if (energy_ == 0.0) field->KillNmo(std::dynamic_pointer_cast<NonMovableObject>(shared_from_this())); // hsraed from this?!
		return true;
	}
	else return false;
}

// Tree

Tree::Tree(double energy) : Plant() {
	default_energy_ = energy;
	energy_ = default_energy_;
}

Tree::~Tree() {
}

// NonPlant


NonPlant::NonPlant() : NonMovableObject() {
	SetType(Type::PLANT, false);
}

// Creature

Creature::Creature() : MovableObject() {
	SetType(Type::CREATURE, true);
}

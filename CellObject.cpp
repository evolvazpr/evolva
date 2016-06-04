#include "Field.hpp"
#include "CellObject.hpp"
#include <cstring>

// CellObject

CellObject::CellObject() : id_(field->GetFfid()) {
	// Zero types
	memset(&types_, 0, sizeof(bool) * 11);
}

CellObject::CellObject(const size_t id) : id_(id) {
	// Zero types
	memset(&types_, 0, sizeof(bool) * 11);
}

CellObject::~CellObject() {
}

bool CellObject::GetType(const CellObject::Type bit) const {
	return types_[static_cast<unsigned char>(bit)];
}

void CellObject::SetType(const CellObject::Type bit, const bool value) {
	types_[static_cast<unsigned char>(bit)] = value;
}

// NonMovableObject

NonMovableObject::NonMovableObject() : CellObject() {
	SetType(Type::MOVABLE, false);
}

NonMovableObject::~NonMovableObject() {
}


// Plant

Plant::Plant(const double energy) : Eatable(0, -7.0, 28.0, 35.0, 5) {
	SetType(Type::PLANT, true);
}

Plant::~Plant() {
}

double Plant::Eat(double energy) {
	if (energy_ - energy <= 0.0) {
		field->KillNmo(std::static_pointer_cast<NonMovableObject>(shared_from_this())); // IT IS non movable
		return energy_; // no need to set energy to 0, because it is already killed
	}
	else {
		energy_ -= energy;
		return energy;
	}
}

// Tree

Tree::Tree(double energy) : Plant(energy) {
	SetType(Type::TREE, true);
	default_energy_ = energy;
	energy_ = default_energy_;
}

Tree::~Tree() {
}

// NonPlant


NonPlant::NonPlant() : NonMovableObject() {
	SetType(Type::PLANT, false);
}

// Flesh

Flesh::Flesh(const double energy) : Eatable(energy, -5.24, 0.0, 0.0, -1) {
	// -5.24 and zeros are chosen because: using this equation
	// simple flesh with 100 energy will stand for 3 turns and
	// disappear on 4th turn
	SetType(Type::FLESH, true);
}

Flesh::~Flesh() {
}

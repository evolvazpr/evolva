#include "Field.hpp"
#include "CellObject.hpp"
#include "Statistics.hpp"
#include <cstring>

// CellObject

//CellObject()
CellObject::CellObject() : id_(field->GetFfid()) {
	// Zero types
	memset(&types_, 0, sizeof(bool) * 11);
}

//CellObject()
CellObject::CellObject(const size_t id) : id_(id) {
	// Zero types
	memset(&types_, 0, sizeof(bool) * 11);
}

//~CellObject()
CellObject::~CellObject() {
}

//GetType()
bool CellObject::GetType(const CellObject::Type bit) const {
	return types_[static_cast<unsigned char>(bit)];
}

//SetType()
void CellObject::SetType(const CellObject::Type bit, const bool value) {
	types_[static_cast<unsigned char>(bit)] = value;
}

// NonMovableObject

NonMovableObject::NonMovableObject() : CellObject() {
	SetType(Type::MOVABLE, false);
	SetType(Type::NON_MOVABLE, true);
}

NonMovableObject::~NonMovableObject() {
}


// Plant

Plant::Plant(const double energy) : Eatable(0, -7.0, 28.0, 35.0, 5) {
	SetType(Type::PLANT, true);
	energy_ = energy;
}

Plant::~Plant() {
}

// Tree

Tree::Tree(const double energy) : Plant(energy) {
	SetType(Type::TREE, true);
	default_energy_ = energy;
	field->stats_->tree_++;
}

Tree::~Tree() {
	field->stats_->tree_--;
}

// Flesh

Flesh::Flesh(const double energy, const bool carnivore) : Eatable(energy, -5.24, 0.0, 0.0, -1) {
	// -5.24 and zeros are chosen because: using this equation
	// simple flesh with 100 energy will stand for 3 turns and
	// disappear on 4th turn
	SetType(Type::FLESH, true);
	carnivore_ = carnivore;
	field->stats_->flesh_++;
}

Flesh::~Flesh() {
	field->stats_->flesh_--;
}

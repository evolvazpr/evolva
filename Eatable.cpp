#include "Field.hpp"
#include "CellObject.hpp"
#include "Eatable.hpp"

double Eatable::Eat(double energy) {
	if (energy_ - energy <= 0.0) {
		field->KillNmo(std::static_pointer_cast<NonMovableObject>(shared_from_this())); // IT IS non movable
		return energy_; // no need to set energy to 0, because it is already killed
	}
	else {
		energy_ -= energy;
		return energy;
	}
}

Eatable::Eatable(const double default_energy, const double growth_a, const double growth_b, const double growth_c, const size_t growth_treshold) : NonMovableObject() {
	SetType(CellObject::Type::EATABLE, true);
	default_energy_ = default_energy;
	energy_ = default_energy;
	growth_a_ = growth_a;
	growth_b_ = growth_b;
	growth_c_ = growth_c;
	growth_treshold_ = growth_treshold;
	turns_ = 0;
}

Eatable::~Eatable() {
}

#include <iostream>

void Eatable::Grow() {
	if (++turns_ <= growth_treshold_) {
		energy_ += (growth_a_ * turns_ * turns_ + growth_b_ * turns_ + growth_c_);
		if (energy_ <= 0.0) {
			field->KillNmo(std::static_pointer_cast<NonMovableObject>(shared_from_this())); // IT IS non movable
		}
	}
}

#include "Field.hpp"
#include "CellObject.hpp"
#include "Tui.hpp"
#define private public
#define protected public
#include "DnaGenerator.hpp"
#include "Unit.hpp"
#undef protected
#undef private
#include <iostream>

std::shared_ptr<Field> field;

int main(void) {

	field = Field::GetInstance(25, 25);

	std::shared_ptr<DnaCode> dna_ptr = std::make_shared<DnaCode>();
	DnaCode &dna = *dna_ptr;
	dna["intelligence"] = 45.0;
	dna["agility"] = 22.0;
	dna["speed"] = 31.0;
	dna["strength"] = 68.4;
	dna["death_time_u"] = 50.0;
	dna["death_time_s"] = 2.68;
	dna["normal_weight"] = 80.0;

	std::cout << "gt base:\n";
	for (auto j = dna.begin(); j != dna.end(); ++j) {
		std::cout << j->first << ": " << j->second << "\n";
	}
	std::cout << "\n";

	DnaGenerator gen(dna_ptr);
	gen.variability_ = 30.0;
	for (size_t i = 0; i < 10; ++i) {
		auto unit = std::make_shared<Unit>(gen.Generate());
		Unit *u = unit.get();
		while(!field->InsertObject(std::move(unit), field->Random()() % field->GetWidth(), field->Random()() % field->GetHeight()));
	}
	field->BeginCycle();
	field->Pause();
	size_t i = 0;
	for (auto u = field->GetCurrentUnit(); !field->IsCycleEnd(); u = field->NextUnit(), ++i) {
		if (!u->GetType(CellObject::Type::UNIT)) continue;
		std::cout << "u " << i << ":\n";
		for (auto j = u->dna_.begin(); j != u->dna_.end(); ++j) {
            std::cout << j->first << ": " << j->second << "\n";
		}
		std::cout << "death: " << u->death_ << "\n";
		std::cout << "energy: " << u->energy_ << "\n";
		std::cout << "c. speed: " << u->speed_ << "\n";
		std::cout << "\n";
	}



	char c = 0;
	std::cin >> c;
	Tui tui;
	tui.PrintField();
	return 0;
}

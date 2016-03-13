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
	std::vector<std::shared_ptr<Unit>> u;
	for (size_t i = 0; i < 10; ++i) {
		u.push_back(std::make_shared<Unit>(gen.Generate()));
		std::cout << "u " << i << ":\n";
		for (auto j = u.back()->dna_.begin();
		j != u.back()->dna_.end();
		++j) {
            std::cout << j->first << ": " << j->second << "\n";
		}
		std::cout << "death: " << u.back()->death_ << "\n";
		std::cout << "energy: " << u.back()->energy_ << "\n";
		std::cout << "c. speed: " << u.back()->speed_ << "\n";
		std::cout << "\n";
	}



	char c = 0;
	std::cin >> c;
	Tui tui;
	std::shared_ptr<MovableObject> movable = std::make_shared<MovableObject>();
	std::shared_ptr<NonMovableObject> non_movable = std::make_shared<NonMovableObject>();
	field->InsertObject(movable, 2, 2);
	field->InsertObject(non_movable, 3, 3);
	tui.PrintField();
	tui.PresentCellObject(movable);
	tui.PresentCellObject(non_movable);
	tui.PresentFieldCell(2, 2);
	tui.PresentFieldCell(1, 1);
	std::cout << "Moving movable to (4, 4)\n";
	try {
		field->MoveObjectTo(movable, 4, 4);
	} catch(EvolvaException &error) {
		std::cout << error.what() << std::endl;
	}
	tui.PrintField();
	return 0;
}

#include "../Field.hpp"
#include "../CellObject.hpp"
#include "../Tui.hpp"
#define private public
#define protected public
#include "../DnaGenerator.hpp"
#include "../Unit.hpp"
#undef protected
#undef private
#include <iostream>
#include <stdio.h>

#include "../CyclicQueue.hpp"
#include <memory>
#include <QApplication>

#include "../dialog.hpp"

void EvolvaInit(Dialog* w) {
	field = Field::GetInstance(10, 10, w);
	std::shared_ptr<DnaCode> dna_ptr = std::make_shared<DnaCode>();
	DnaCode &dna = *dna_ptr;
	dna["intelligence"] = 45.0;
	dna["agility"] = 22.0;
	dna["speed"] = 31.0;
	dna["strength"] = 68.4;
	dna["death_time_u"] = 50.0;
	dna["death_time_s"] = 2.68;
	dna["mutability"] = 10.0;
	dna["mutation_stability"] = 1.1;
	dna["fatigue_death_threshold"] = 80.0;
	dna["health_death_threshold"] = 20.0;
	dna["fatigue_regeneration"] = 10.0;
	dna["health_regeneration_sleep"] = 5.0;
	dna["requirements.normal_turn"] = 10.0;
	dna["requirements.sleep_turn"] = 3.0;
	dna["requirements.eating"] = 2.0;
	dna["requirements.mating"] = 25.0;
	dna["requirements.step"] = 1.0;
	dna["requirements.attack"] = 50.0;
	dna["requirements.childbirth"] = 80.0;
	dna["wakeup_threshold"] = 20.0;
	dna["fatigue_turn_increase"] = 2.0;
	dna["normal_weight"] = 80.0;
	dna["pregnancy_time"] = 2.0;
	dna["herbivore"] = 100.0;
	dna["carnivore"] = 0.0;

	DnaGenerator gen(dna_ptr);
	gen.variability_ = 30.0;

	std::shared_ptr<Unit> u[2];
	u[0] = std::make_shared<Unit>(gen.Generate());
	u[1] = std::make_shared<Unit>(gen.Generate());

	u[0]->energy_ = 200.0;
	u[0]->death_ = 10000;
	u[0]->dna_["normal_weight"] = 220.0;
	field->InsertObject(u[0], 6, 6);
	u[1]->energy_ = 200.0;
	u[1]->death_ = 10000;
	u[1]->dna_["normal_weight"] = 200.0;
	field->InsertObject(u[1], 7, 7);


/*	for (size_t i = 0; i < 2; ++i) {
		if (!u[i]->GetType(CellObject::Type::UNIT)) continue;
		std::cout << "u " << i << ":\n";
		for (auto j = u[i]->dna_.begin(); j != u[i]->dna_.end(); ++j) {
			std::cout << j->first << ": " << j->second << "\n";
		}
		std::cout << "death: " << u[i]->death_ << "\n";
		std::cout << "energy: " << u[i]->energy_ << "\n";
		std::cout << "c. speed: " << u[i]->speed_ << "\n";
		std::cout << "x: " << u[i]->GetX() << "\n";
		std::cout << "y: " << u[i]->GetY() << "\n";
		std::cout << "\n";
	}*/


	field->InsertNmo(std::make_shared<Tree>(50.0), 0, 0);
	field->InsertNmo(std::make_shared<Tree>(80.0), 0, 9);
	field->InsertNmo(std::make_shared<Tree>(20.0), 9, 0);
	field->InsertNmo(std::make_shared<Tree>(24.0), 9, 9);
	field->InsertNmo(std::make_shared<Tree>(500.0), 1, 0);
//	field->InsertNmo(std::make_shared<Tree>(100.0), 5, 5);/**/

	field->BeginCycle();
	field->Play();
	Unit *xz = u[0].get();
	xz->IsRemoved();
}

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	Dialog w;
	EvolvaInit(&w);
	w.show();
	a.exec();
	return 0;
}

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

void create_herbivore() {
		std::shared_ptr<DnaCode> dna_ptr = std::make_shared<DnaCode>();
		std::shared_ptr<Field> field = Field::GetInstance(10, 10);
		DnaCode &dna = *dna_ptr;
		dna["intelligence"] = 80.0;
		dna["agility"] = 30.0;
		dna["speed"] = 80.0;
		dna["strength"] = 20.4;
		dna["death_time_u"] = 50.0;
		dna["death_time_s"] = 2.68;
		dna["mutability"] = 10.0;
		dna["mutation_stability"] = 1.1;
		dna["fatigue_death_threshold"] = 80.0;
		dna["health_death_threshold"] = 20.0;
		dna["fatigue_regeneration"] = 10.0;
		dna["health_regeneration_sleep"] = 5.0;
		dna["health_regeneration"] = 0.0;
		dna["requirements.normal_turn"] = 10.0;
		dna["requirements.sleep_turn"] = 10.0;
		dna["requirements.eating"] = 2.0;
		dna["requirements.mating"] = 25.0;
		dna["requirements.step"] = 2.0;
		dna["requirements.attack"] = 50.0;
		dna["requirements.childbirth"] = 40.0;
		dna["wakeup_threshold"] = 20.0;
		dna["fatigue_turn_increase"] = 2.0;
		dna["normal_weight"] = 80.0;
		dna["pregnancy_time"] = 2.0;
		dna["herbivore"] = 100.0;
		dna["carnivore"] = 0.0;
		dna["vision"] = 15.0;

		DnaGenerator gen(dna_ptr);
		gen.variability_ = 30.0;
		std::shared_ptr<Unit> herb = std::make_shared<Unit>(gen.Generate());
		field->InsertObject(herb, 5, 5);
}

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	Dialog *gui = Dialog::GetInstance(nullptr, 10, 10);
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			gui->CreateSurfaceObject(FieldCell::Ground::GROUND, i, j);			
		}
	}
	create_herbivore();
	for (int i = 4; i < 7; i++) {
		for (int j = 4; j < 7; j++) {
			gui->RemoveSurfaceObject(i, j);
			gui->CreateSurfaceObject(FieldCell::Ground::GRASS, i, j);
		}
	}

	gui->show();
	a.exec();
	return 0;
}

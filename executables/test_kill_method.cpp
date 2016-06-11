#include "../Field.hpp"
#include "../CellObject.hpp"
#include "../DnaGenerator.hpp"
#include "../Unit.hpp"
#include <iostream>
#include <stdio.h>

#include "../CyclicQueue.hpp"
#include <memory>
#include "../Application.hpp"

#include "../dialog.hpp"
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE insert_objects

#include <boost/test/unit_test.hpp>


/**
 * This is a unit test for object removing from map, and 
 * clearing space after it.
 */

BOOST_AUTO_TEST_CASE(kill)
{
	int argc = 1;
	char *argv[2];
	argv[0] = (char *)"kill"; //only for creating QApplication
	Application *a = Application::GetInstance(argc, argv);
	a->Init(0);
	
	std::shared_ptr<Field> field = Field::GetInstance(10, 10);
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

	std::shared_ptr<Unit> u;
	u = std::make_shared<Unit>(gen.Generate());

	BOOST_CHECK(field->InsertObject(u, 6, 6) == true);
	
	BOOST_CHECK(field->GetCell(6, 6)->GetObject()->GetType(CellObject::Type::FLESH) == false);
	field->Kill(u, 8);
	BOOST_CHECK(field->GetCell(6, 6)->GetObject()->GetType(CellObject::Type::FLESH) == true);
	field->KillNmo(field->GetCell(6, 6)->GetNmo());
	BOOST_CHECK(field->GetCell(6,6)->IsEmpty() == true);	

	delete(a);
}

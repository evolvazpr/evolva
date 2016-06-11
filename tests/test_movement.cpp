#include "../Field.hpp"
#include "../CellObject.hpp"
#include "../Tui.hpp"
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
 * This is a unit test for object movement. It checks if 
 * movement and "collision" detection works.
 */

BOOST_AUTO_TEST_CASE(movement)
{
	int argc = 1;
	char *argv[2];
	argv[0] = (char *)"insert"; //only for QApplication creation
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

	std::shared_ptr<Unit> u[2];
	u[0] = std::make_shared<Unit>(gen.Generate());
	u[1] = std::make_shared<Unit>(gen.Generate());

	BOOST_CHECK(field->InsertObject(u[0], 6, 6) == true);

	BOOST_CHECK(field->InsertObject(u[1], 7, 7) == true);

	BOOST_CHECK(field->InsertNmo(std::make_shared<Tree>(50.0), 0, 0) == true);
	BOOST_CHECK(field->InsertNmo(std::make_shared<Tree>(80.0), 0, 9) == true);
	BOOST_CHECK(field->InsertNmo(std::make_shared<Tree>(20.0), 9, 0) == true);
	BOOST_CHECK(field->InsertNmo(std::make_shared<Tree>(24.0), 9, 9) == true);
	BOOST_CHECK(field->InsertNmo(std::make_shared<Tree>(500.0), 1, 0) == true);
	BOOST_CHECK(field->InsertNmo(std::make_shared<Tree>(100.0), 1, 0) == false);

	BOOST_CHECK(field->MoveObject(u[0], -1, -1, 1) == true);
	BOOST_CHECK(field->MoveObject(u[1], -2, -2, 1) == false);
	BOOST_CHECK(field->MoveObject(u[1], 1, 1, 1) == true);
	BOOST_CHECK(field->MoveObjectTo(u[0], 0, 0, 1) == false);
	BOOST_CHECK(field->MoveObjectTo(u[0], 1, 1, 1) == true);
	BOOST_CHECK(field->MoveObjectTo(u[1], 1, 1, 1) == false);
	BOOST_CHECK(field->MoveObjectTo(u[1], 0, 5, 1) == true);

	delete(a);
}

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
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE insert_objects

#include <boost/test/unit_test.hpp>

void DnaInit(DnaCode &dna)
{
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
}

void EvolvaInit() {
	field = Field::GetInstance(10, 10);
	std::shared_ptr<DnaCode> dna_herb_ptr = std::make_shared<DnaCode>();
	std::shared_ptr<DnaCode> dna_carn_ptr = std::make_shared<DnaCode>();

	DnaCode &dna_herb = *dna_herb_ptr;
	DnaCode &dna_carn = *dna_carn_ptr;

	DnaInit(dna_herb);
	DnaInit(dna_carn);

	dna_herb["herbivore"] = 100.0;
	dna_herb["carnivore"] = 0.0;

	dna_carn["herbivore"] = 0.0;
	dna_carn["carnivore"] = 100.0;

	DnaGenerator gen_herb(dna_herb_ptr);
	DnaGenerator gen_carn(dna_carn_ptr);

	std::shared_ptr<Unit> u[2];
	std::shared_ptr<Tree> t[2];
	u[0] = std::make_shared<Unit>(gen_herb.Generate());
	u[1] = std::make_shared<Unit>(gen_carn.Generate());

	t[0] = std::make_shared<Tree>(50.0);
	t[1] = std::make_shared<Tree>(50.0);

	BOOST_CHECK(field->InsertObject(u[0], 6, 5) == true);
	BOOST_CHECK(field->InsertObject(u[1], 7, 6) == true);
	BOOST_CHECK(field->InsertObject(t[0], 0, 1) == true);
	BOOST_CHECK(field->InsertObject(t[1], 1, 9) == true);

	BOOST_CHECK(u[0]->GetType(CellObject::Type::MOVABLE) == true);
	BOOST_CHECK(u[0]->GetType(CellObject::Type::NON_MOVABLE) == false);
	BOOST_CHECK(u[0]->GetType(CellObject::Type::PLANT) == false);
	BOOST_CHECK(u[0]->GetType(CellObject::Type::NON_PLANT) == false);
	BOOST_CHECK(u[0]->GetType(CellObject::Type::FLESH) == false);
	BOOST_CHECK(u[0]->GetType(CellObject::Type::UNIT) == true);
	BOOST_CHECK(u[0]->GetType(CellObject::Type::CARNIVORE) == false);
	BOOST_CHECK(u[0]->GetType(CellObject::Type::HERBIVORE) == true);
	BOOST_CHECK(u[0]->GetType(CellObject::Type::CREATURE) == true);

	BOOST_CHECK(u[1]->GetType(CellObject::Type::MOVABLE) == true);
	BOOST_CHECK(u[1]->GetType(CellObject::Type::NON_MOVABLE) == false);
	BOOST_CHECK(u[1]->GetType(CellObject::Type::PLANT) == false);
	BOOST_CHECK(u[1]->GetType(CellObject::Type::NON_PLANT) == false);
	BOOST_CHECK(u[1]->GetType(CellObject::Type::FLESH) == false);
	BOOST_CHECK(u[1]->GetType(CellObject::Type::UNIT) == true);
	BOOST_CHECK(u[1]->GetType(CellObject::Type::CARNIVORE) == true);
	BOOST_CHECK(u[1]->GetType(CellObject::Type::HERBIVORE) == false);
	BOOST_CHECK(u[1]->GetType(CellObject::Type::CREATURE) == true);

	BOOST_CHECK(t[0]->GetType(CellObject::Type::MOVABLE) == false);
	BOOST_CHECK(t[0]->GetType(CellObject::Type::NON_MOVABLE) == true);
	BOOST_CHECK(t[0]->GetType(CellObject::Type::PLANT) == true);
	BOOST_CHECK(t[0]->GetType(CellObject::Type::NON_PLANT) == false);
	BOOST_CHECK(t[0]->GetType(CellObject::Type::FLESH) == false);
	BOOST_CHECK(t[0]->GetType(CellObject::Type::UNIT) == false);
	BOOST_CHECK(t[0]->GetType(CellObject::Type::CARNIVORE) == false);
	BOOST_CHECK(t[0]->GetType(CellObject::Type::HERBIVORE) == false);
	BOOST_CHECK(t[0]->GetType(CellObject::Type::CREATURE) == false);

	BOOST_CHECK(t[1]->GetType(CellObject::Type::MOVABLE) == false);
	BOOST_CHECK(t[1]->GetType(CellObject::Type::NON_MOVABLE) == true);
	BOOST_CHECK(t[1]->GetType(CellObject::Type::PLANT) == true);
	BOOST_CHECK(t[1]->GetType(CellObject::Type::NON_PLANT) == false);
	BOOST_CHECK(t[1]->GetType(CellObject::Type::FLESH) == false);
	BOOST_CHECK(t[1]->GetType(CellObject::Type::UNIT) == false);
	BOOST_CHECK(t[1]->GetType(CellObject::Type::CARNIVORE) == false);
	BOOST_CHECK(t[1]->GetType(CellObject::Type::HERBIVORE) == false);
	BOOST_CHECK(t[1]->GetType(CellObject::Type::CREATURE) == false);
}

BOOST_AUTO_TEST_CASE(GetType)
{
	int argc = 1;
	char *argv[2];
	argv[0] = (char *)"insert";
	QApplication a(argc, argv); //only for QApplication creation
	Dialog::GetInstance(); //no need to get Instance, right now only initalization
	EvolvaInit();
}

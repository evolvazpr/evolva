#include "../CyclicQueue.hpp"
#include "../CellObject.hpp"
#include "../Field.hpp"
#define private public
#define protected public
#include "../DnaGenerator.hpp"
#include "../Unit.hpp"
#undef protected
#undef private
#include "../dialog.hpp"
#include <QApplication>


#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE insert_objects

#include <boost/test/unit_test.hpp>

/**
 * This is a unit test for CyclicQueue
 */

BOOST_AUTO_TEST_CASE(CyclicQueueUnitTest)
{
	int argc = 1;
	char *argv[2];
	argv[0] = (char *)"insert"; //only for QApplication creation
	QApplication a(argc, argv);
	Dialog w;

	field = Field::GetInstance(10, 10, &w); 
	CyclicQueue queue;
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
	queue.Begin();
	queue.Insortion(u[0]);
	queue.Insortion(u[1]);
	/*TODO: Don't know how to test it - right now all 
	 * movable objects have MovePriority parameter which 
	 * equals to 0.
	 */
	queue.Begin();
	BOOST_CHECK(queue.Next() == false);
	BOOST_CHECK(queue.IsEnd() == false);
	BOOST_CHECK(queue.Get() == u[0]);
	BOOST_CHECK(queue.Next() == true);
	BOOST_CHECK(queue.IsEnd() == true);
	BOOST_CHECK(queue.Get() == u[1]);
	BOOST_CHECK(queue.Next() == false);
	BOOST_CHECK(queue.IsEnd() == false);
	BOOST_CHECK(queue.Get() == u[0]);	
} 

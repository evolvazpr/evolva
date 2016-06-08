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

/**
 * This is a unit test for nonmovable object remove.
 */

void KillNmoTest(std::shared_ptr <Unit> u, std::shared_ptr<Tree> t, int x, int y) {
	BOOST_CHECK(field->MoveObjectTo(u, x, y, 1) == false);
	field->KillNmo(t);
	BOOST_CHECK(field->MoveObjectTo(u, x, y, 1) == true);	
}

BOOST_AUTO_TEST_CASE(kill)
{
	int argc = 1;
	char *argv[2];
	argv[0] = (char *)"kill"; //only for creating QApplication
	QApplication a(argc, argv);
	Dialog::GetInstance();

	field = Field::GetInstance(10, 10);
	std::shared_ptr<DnaCode> dna_ptr = std::make_shared<DnaCode>();
	DnaCode &dna = *dna_ptr;
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
	BOOST_CHECK(field->InsertObject(u[0], 6, 6) == true);
	u[1]->energy_ = 200.0;
	u[1]->death_ = 10000;
	u[1]->dna_["normal_weight"] = 200.0;
	BOOST_CHECK(field->InsertObject(u[1], 7, 7) == true);


	std::shared_ptr<Tree> tree = std::make_shared<Tree>(50.0);
	BOOST_CHECK(field->InsertNmo(tree, 0, 0) == true);
	BOOST_CHECK(field->InsertNmo(std::make_shared<Tree>(80.0), 0, 9) == true);
	BOOST_CHECK(field->InsertNmo(std::make_shared<Tree>(20.0), 9, 0) == true);
	BOOST_CHECK(field->InsertNmo(std::make_shared<Tree>(24.0), 9, 9) == true);
	BOOST_CHECK(field->InsertNmo(std::make_shared<Tree>(500.0), 1, 0) == true);
	BOOST_CHECK(field->InsertNmo(std::make_shared<Tree>(100.0), 1, 0) == false);
	KillNmoTest(u[0], tree, 0, 0);

}

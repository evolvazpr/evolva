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

BOOST_AUTO_TEST_CASE(insert)
{
	int argc = 1;
	char *argv[2];
	argv[0] = (char *)"insert";
	Application *a = Application::GetInstance(argc, argv); 
	a->Init(0);
	field = Field::GetInstance(10, 10);
	std::shared_ptr<DnaCode> dna_ptr = std::make_shared<DnaCode>();

	DnaGenerator gen(dna_ptr);

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
}

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

void EvolvaInit() {

}

BOOST_AUTO_TEST_CASE(GetCoord)
{
	int argc = 1;
	char *argv[2];
	argv[0] = (char *)"insert";
	QApplication a(argc, argv); //only for QApplication creation
	Dialog* w = Dialog::GetInstance(nullptr, 10, 10);
	(void)w;

	field = Field::GetInstance(10, 10); 
	//somewhere is defined global std::shared_ptr<Field> field.
	//I think this global should be removed. (Konrad Gotfryd)
	std::shared_ptr<DnaCode> dna_ptr = std::make_shared<DnaCode>();

	DnaGenerator gen(dna_ptr);

	std::shared_ptr<Unit> u[2];
	std::shared_ptr<Tree> t[2];
	u[0] = std::make_shared<Unit>(gen.Generate());
	u[1] = std::make_shared<Unit>(gen.Generate());

	t[0] = std::make_shared<Tree>(50.0);
	t[1] = std::make_shared<Tree>(50.0);

	BOOST_CHECK(field->InsertObject(u[0], 6, 5) == true);
	BOOST_CHECK(field->InsertObject(u[1], 7, 6) == true);
	BOOST_CHECK(field->InsertObject(t[0], 0, 1) == true);
	BOOST_CHECK(field->InsertObject(t[1], 1, 9) == true);

	BOOST_CHECK(u[0]->GetX() == 6);
	BOOST_CHECK(u[0]->GetY() == 5);
	BOOST_CHECK(u[1]->GetX() == 7);
	BOOST_CHECK(u[1]->GetY() == 6);
	BOOST_CHECK(t[0]->GetX() == 0);
	BOOST_CHECK(t[0]->GetY() == 1);
	BOOST_CHECK(t[1]->GetX() == 1);
	BOOST_CHECK(t[1]->GetY() == 9);

}

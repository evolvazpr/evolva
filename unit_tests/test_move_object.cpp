#include "../Field.hpp"
#include "../Unit.hpp"
#include "../DnaGenerator.hpp"
#include "../Tui.hpp"

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE move_on_field
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(move) {
	DnaGenerator dna_generator;
	std::shared_ptr<Field> field = Field::GetInstance(12, 12);
	dna_generator.variability_ = 0.123;
	std::shared_ptr<DnaCode> dna_code = dna_generator.Generate();
	std::shared_ptr<MovableObject> mambo = std::make_shared<MovableObject>(Unit(dna_code));
	std::shared_ptr<MovableObject> bambo = std::make_shared<MovableObject>(Unit(dna_code));
	field->InsertObject(mambo, 10, 10);
	field->InsertObject(bambo, 2, 3);
	BOOST_CHECK(field->MoveObjectTo(mambo, 2, 3) == false);
	BOOST_CHECK(field->MoveObjectTo(mambo, 2, 2) == true);
	BOOST_CHECK(field->GetCell(2, 2)->GetObject() == mambo);
	BOOST_CHECK(field->GetCell(10, 10)->GetObject() == nullptr);
	BOOST_CHECK(field->MoveObject(bambo, 0, -1) == false);
	BOOST_CHECK(field->MoveObject(bambo, 0, -2) == true);
	BOOST_CHECK(field->GetCell(2, 1)->GetObject() == bambo);
	BOOST_CHECK(field->GetCell(2, 3)->GetObject() == nullptr);
}

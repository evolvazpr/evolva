#include "Field.hpp"
#include "Unit.hpp"
#include "DnaGenerator.hpp"
#include "Tui.hpp"

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Insert_into_field
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(insert) {
	DnaGenerator dna_generator;
	std::shared_ptr<Field> field = Field::GetInstance(12, 12);
	dna_generator.variability_ = 0.123;
	std::shared_ptr<DnaCode> dna_code = dna_generator.Generate();
	std::shared_ptr<MovableObject> mambo = std::make_shared<MovableObject>(Unit(dna_code));
	std::shared_ptr<MovableObject> bambo = std::make_shared<MovableObject>(Unit(dna_code));
	BOOST_CHECK(field->InsertObject(mambo, 10, 10) == 1);
	BOOST_CHECK(field->InsertObject(bambo, 10, 10) == 0);
	BOOST_CHECK(field->InsertObject(bambo, 2, 3) == 1);
}

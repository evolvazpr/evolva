#include "Field.hpp"
#include "CellObject.hpp"

void test(int rt) {
	std::cout << "Error: " << rt << "\n";
	exit(rt);
}

int main(void) {
	bool ret;
	int rt;

	std::shared_ptr<CellObject> tree = std::make_shared<Plant>(5, false);
	std::shared_ptr<CellObject> human = std::make_shared<TestUnit>();
	std::shared_ptr<CellObject> cat = std::make_shared<TestUnit>();

	Field &field = Field::getInstance(10, 10);


	ret = field.addObject(tree, 4, 4);
	if (!ret) 
		test(1);

	ret = field.addObject(human, 5, 5);
	if (!ret)
		test(2);

	ret = field.addObject(cat, 6, 6);
		if (!ret)
			test(3);

	ret = field.moveObjectWithRelativeStep(tree, 1, 2);
	if (ret)
		test(4);
	
	ret = field.moveObjectWithRelativeStep(human, -2, -2);
	if (!ret)
		test(5);

	ret = field.moveObjectWithRelativeStep(cat, -3, -3);
	if (ret)
		test(6);

	ret = field.moveObjectWithRelativeStep(human, 1, -1);
	if (!ret)
		test(7);

	ret = field.moveObjectWithRelativeStep(cat, -3, -3);
	if (!ret)
		test(8);

	field.removeObject(tree);

	field.removeObject(cat);

	ret = field.moveObjectWithRelativeStep(cat, -2, -2);
	if (ret) 
		test(9);

	field.removeObject(human);
	
	std::cout << "No error.\n";
	return 0;
}


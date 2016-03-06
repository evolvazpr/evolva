#include "Field.hpp"
#include "CellObject.hpp"

void test(int rt) {
	std::cout << "Error: " << rt << "\n";
	exit(rt);
}

int main(void) {
	bool ret;

	std::shared_ptr<CellObject> tree = std::make_shared<Plant>(5, false);
	std::shared_ptr<CellObject> human = std::make_shared<TestUnit>();
	std::shared_ptr<CellObject> cat = std::make_shared<TestUnit>();

	Field &field = Field::GetInstance(10, 10);


	ret = field.AddObject(tree, 4, 4);
	if (!ret) 
		test(1);

	ret = field.AddObject(human, 5, 5);
	if (!ret)
		test(2);

	ret = field.AddObject(cat, 6, 6);
		if (!ret)
			test(3);

	ret = field.MoveObjectWithRelativeStep(tree, 1, 2);
	if (ret)
		test(4);
	
	ret = field.MoveObjectWithRelativeStep(human, -2, -2);
	if (!ret)
		test(5);

	ret = field.MoveObjectWithRelativeStep(cat, -3, -3);
	if (ret)
		test(6);

	ret = field.MoveObjectWithRelativeStep(human, 1, -1);
	if (!ret)
		test(7);

	ret = field.MoveObjectWithRelativeStep(cat, -3, -3);
	if (!ret)
		test(8);

	field.RemoveObject(tree);

	field.RemoveObject(cat);

	ret = field.MoveObjectWithRelativeStep(cat, -2, -2);
	if (ret) 
		test(9);

	field.RemoveObject(human);
	
	std::cout << "No error.\n";
	return 0;
}


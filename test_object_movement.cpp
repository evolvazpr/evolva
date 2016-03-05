#include "Field.hpp"
#include "CellObject.hpp"

int main(void) {
	bool ret;
	int rt;

	std::shared_ptr<CellObject> tree = std::make_shared<Plant>(5, false);
	std::shared_ptr<CellObject> human = std::make_shared<test>();
	std::shared_ptr<CellObject> cat = std::make_shared<test>();
	Field &field = Field::getInstance(10, 10);

	ret = field.addObjectToWorld(tree, 4, 4);
	if (!ret) {
		rt = 1;
		goto err;//just temporary goto
	}

	ret = field.addObjectToWorld(human, 5, 5);
	if (!ret) {
		rt = 2;
		goto err;
	}

	ret = field.addObjectToWorld(cat, 6, 6);
		if (!ret) {
		rt = 3;
		goto err;
	}

	ret = field.moveObjectWithRelativeStep(tree, 1, 2);
	if (ret) {
		rt = 4;
		goto err; 
	}
	
	ret = field.moveObjectWithRelativeStep(human, -2, -2);
	if (!ret) {
		rt = 5;
		goto err;
	}

	ret = field.moveObjectWithRelativeStep(cat, -3, -3);
	if (ret) {
		rt = 6;
		goto err;
	}

	ret = field.moveObjectWithRelativeStep(human, 1, -1);
	if (!ret) {
		rt = 7;
		goto err;
	}
	ret = field.moveObjectWithRelativeStep(cat, -3, -3);
	if (!ret) {
		rt = 8;
		goto err;
	}

	field.removeObjectFromWorld(tree);

	field.removeObjectFromWorld(cat);

	ret = field.moveObjectWithRelativeStep(cat, -2, -2);
	if (ret) {
		rt = 9;
		goto err;
	}

	field.removeObjectFromWorld(human);
	
	std::cout << "No error.\n";
	return 0;
err:

	field.removeObjectFromWorld(tree);
	field.removeObjectFromWorld(cat);
	field.removeObjectFromWorld(human);
	std::cout << "Error: " << rt << "\n";
	return rt;
}


#include "Field.hpp"

/*
 * TEST SCENARIO:
 * 1. Two objects on a map. First on (4, 4) coordinate, second on (5, 5).
 * 2. Moving first object to (5, 6), checking everything.
 * 3. Moving first object to (5, 5), checking eveyrthing, should fail.
 * 4. Moving second object to (6, 6), checking everything.
 * 5. Moving first object to (5, 5), checking everything.
 *
 * Testing if not movable object is moved is not implemented yet.
 */
int main(void) {
	bool ret;
	int rt;

	std::shared_ptr<CellObject> first_obj = std::make_shared<Tree>();
	std::shared_ptr<CellObject> second_obj = std::make_shared<Tree>();
	Field &field = Field::getInstance(10, 10);
	std::shared_ptr<FieldCell> cell1(field.getCell(4, 4));
	std::shared_ptr<FieldCell> cell2(field.getCell(5, 5));

	field.addObjectToVector(first_obj, 4, 4);
	field.addObjectToVector(second_obj, 5, 5);

	if (!cell1) { 
		rt = 1;
		goto err;
	}

	if (!cell2) {
		rt = 2;
		goto err;
	}

	ret = cell1->insertObject(first_obj);
	if (!ret) {
		rt = 3;
		goto err;
	}

	ret = cell2->insertObject(second_obj);
	if (!ret) {
		rt = 4;
		goto err;
	}
	
	ret = field.moveObjectWithRelativeStep(first_obj, 1, 2);
	if (!ret) {
		rt = 5;
		goto err;
	}
	
	ret = field.moveObjectWithRelativeStep(first_obj, 0, -1);
	if (ret) {
		rt = 6;
		goto err;
	}

	ret = field.moveObjectWithRelativeStep(second_obj, 1, 1);
	if (!ret) {
		rt = 7;
		goto err;
	}

	ret = field.moveObjectWithRelativeStep(first_obj, 0, -1);
	if (!ret) {
		rt = 8;
		goto err;
	}

	cell1->removeObject();
	cell2->removeObject();

	return 0;
err:
	std::cout << "Error: " << rt << "\n";
	return rt;
}


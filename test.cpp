#include <iostream>

#include "CellObject.hpp"
#include "Field.hpp"

void print_error(int ret) {
	std::cout << "Error number: " << ret <<".\n";
	exit(1);
}

int main(void) {
	std::shared_ptr<Field> field = Field::GetInstance(10, 10);
	std::shared_ptr<MovableObject> cat = std::make_shared<MovableObject>(0, 3);
	std::shared_ptr<MovableObject> dog = std::make_shared<MovableObject>(1, 3);
	std::shared_ptr<MovableObject> human = std::make_shared<MovableObject>(2, 3);
	std::shared_ptr<NonMovableObject> tree = std::make_shared<NonMovableObject>(3);
	bool ret = field->InsertObject(cat, 5, 5);
	if (!ret)
		print_error(1);
	ret = field->InsertObject(dog, 4, 5);
	if (!ret)
		print_error(2);
	ret = field->InsertObject(human, 4, 5);
	if (ret)
		print_error(3);
	ret = field->InsertObject(human, 5, 5);
	if (ret)
		print_error(4);
	ret = field->InsertObject(human, 6, 5);
	if (!ret)
		print_error(5);
	ret = field->InsertObject(tree, 6, 5);
	if (ret)
		print_error(6);
	ret = field->InsertObject(tree, 7, 5);
	if (!ret)
		print_error(7);


	ret = cat->MoveWithRelativeStep(-1, 0);
	if (ret)
		print_error(7);
	ret = cat->MoveWithRelativeStep(1, 0);
	if (ret)
		print_error(8);
	ret = cat->MoveWithRelativeStep(2, 0);
	if (ret)
		print_error(9);
	ret = cat->MoveWithRelativeStep(3, 0);
	if (!ret)
		print_error(10);	
	field->IterateOverMovableObjects();
	std::cout << "No errors!\n";
	return 0;
}

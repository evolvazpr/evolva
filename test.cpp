#include "Field.hpp"
#include "Tui.hpp"

int main(void) {
	Tui tui;
	std::shared_ptr<Field> field = Field::GetInstance(25, 25);
	std::shared_ptr<MovableObject> movable = std::make_shared<MovableObject>(1, 1);
	std::shared_ptr<NonMovableObject> non_movable = std::make_shared<NonMovableObject>(2);
	field->InsertObject(movable, 2, 2);
	field->InsertObject(non_movable, 3, 3);
	tui.PrintField();
	tui.PresentCellObject(movable);
	tui.PresentCellObject(non_movable);
	tui.PresentFieldCell(2, 2);
	tui.PresentFieldCell(1, 1);
	std::cout << "Moving movable to (4, 4)\n";
	try {
		field->MoveObjectTo(movable, 4, 4);
	} catch(EvolvaException &error) {
		std::cout << error.what() << std::endl;
	}
	tui.PrintField();
	return 0;
}

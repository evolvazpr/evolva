#include "Field.hpp"
#include "TUI.hpp"

int main(void) {
	TUI tui;
	std::shared_ptr<Field> field = Field::GetInstance(10, 10);
	field->InsertObject(std::make_shared<MovableObject>(1, 1), 2, 2);
	field->InsertObject(std::make_shared<NonMovableObject>(2), 3, 3);
	tui.PrintMap();
	return 0;
}

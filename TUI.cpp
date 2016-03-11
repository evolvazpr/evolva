#include "TUI.hpp"

void TUI::PrintMap() {
	std::shared_ptr<Field> instance = Field::GetInstance();
	for (size_t i = 0; i < instance->width_; i++) {
		for (size_t j = 0; j < instance->height_; j++) {
				if (instance->cells_[i][j]->IsEmpty())
					std::cout << "|   ";
				else 
					std::cout << "| X ";
		}
		std::cout << "|\n";
	}
}

void TUI::PresentFieldCell(std::shared_ptr<FieldCell> cell) {
	std::cout << "Presenting cell:\n";
	if (cell->IsEmpty()) std::cout << "Cell is empty.\n";
	else std::cout << "Cell is not empty.\n\n";
}

void TUI::PresentCellObject(std::shared_ptr<CellObject> object) {
	std::cout << "ID: " << object->id_ << std::endl;
	std::cout << "Coordiantes: x = " << object->x_ << ", y = " << object->y_ << ".\n\n";
}

void TUI::PresentMovableObject(std::shared_ptr<MovableObject> object) {
	std::cout << "Presenting moveable object:\n";
	PresentCellObject(static_cast<std::shared_ptr<CellObject>>(object));
	std::cout << "Move priority: " << object->move_priority_ << std::endl;
}

void TUI::PresentNonMovableObject(std::shared_ptr<NonMovableObject> object) {
	std::cout << "Presenting non moveable object:\n\n";
	PresentCellObject(static_cast<std::shared_ptr<CellObject>>(object));
}


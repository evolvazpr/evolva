#include "Tui.hpp"

void Tui::DrawHorizontalLine() {
	const std::shared_ptr<Field> field = Field::GetInstance();
	for (size_t i = 0; i <= field->width_ * 4; i++)
		std::cout << "-";
	std::cout << std::endl;
}

void Tui::PrintField() {
	const std::shared_ptr<Field> field = Field::GetInstance();
	std::shared_ptr<CellObject> object;
	DrawHorizontalLine();
	for (size_t i = 0; i < field->width_; i++) {
		for (size_t j = 0; j < field->height_; j++) {
			if (field->cells_[i][j]->IsEmpty()) {
				std::cout << "|   ";
			}	else {
				object = field->cells_[i][j]->CopyPtr();
				switch(object->GetType()) {	
					case(CellObject::Type::MOVABLE):
						std::cout << "| O ";
						break;
						case(CellObject::Type::NONMOVABLE):
						std::cout << "| X ";
						break;
					default:
						std::cout << "| ? ";
						break;
				}
			}
		}
		std::cout << "|\n";
	}
	DrawHorizontalLine();	
}

void Tui::PresentFieldCell(const size_t x, const size_t y) {
	std::cout << "Presenting cell:\n";
	std::cout << "Coordinates: x = " << x << ", " << y << ".\n";
	if (Field::GetInstance()->cells_[x][y]->IsEmpty()) std::cout << "Cell is empty.\n\n";
	else std::cout << "Cell is not empty.\n\n";
}

void Tui::PresentCellObject(const std::shared_ptr<CellObject> object) {
	std::cout << "ID: " << object->id_ << std::endl;
	std::cout << "Coordiantes: x = " << object->x_ << ", y = " << object->y_ << ".\n";
}

void Tui::PresentCellObject(const std::shared_ptr<MovableObject> object) {
	std::cout << "Presenting moveable object:\n";
	PresentCellObject(static_cast<std::shared_ptr<CellObject>>(object));
	std::cout << "Move priority: " << object->move_priority_ << "\n\n";
}

void Tui::PresentCellObject(const std::shared_ptr<NonMovableObject> object) {
	std::cout << "Presenting non-moveable object:\n";
	PresentCellObject(static_cast<std::shared_ptr<CellObject>>(object));
	std::cout << std::endl;
}


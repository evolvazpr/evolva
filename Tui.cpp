#include "Field.hpp"
#include "FieldCell.hpp"
#include "CellObject.hpp"
#include "Tui.hpp"
#include <iostream>

void Tui::DrawHorizontalLine() {
	const std::shared_ptr<Field> field = Field::GetInstance();
	for (size_t i = 0; i <= field->GetWidth() * 4; i++)
		std::cout << "-";
	std::cout << std::endl;
}
/**
 * Method draws field in console. Objects are represented by 'O' sign.
 */
void Tui::PrintField() {
	const std::shared_ptr<Field> field = Field::GetInstance();
	std::shared_ptr<CellObject> object;
	DrawHorizontalLine();
	for (size_t i = 0; i < field->GetWidth(); i++) {
		for (size_t j = 0; j < field->GetHeight(); j++) {
			if (field->GetCell(i, j)->IsEmpty()) {
				std::cout << "|   ";
			}
			else {
				object = field->GetCell(i, j)->CopyPtr();
				if (object->GetType(CellObject::Type::MOVABLE)) {
					std::cout << "| O ";
				}
				else std::cout << "| O ";
			}
		}
		std::cout << "|\n";
	}
	DrawHorizontalLine();
}

/**
 * Method reports if cell on \p x and \p y coordinates is empty or not - 
 * by occupied we mean that kind of object is standing there.
 */
void Tui::PresentFieldCell(const size_t x, const size_t y) {
	std::cout << "Presenting cell:\n";
	std::cout << "Coordinates: x = " << x << ", " << y << ".\n";
	if (Field::GetInstance()->GetCell(x, y)->IsEmpty()) std::cout << "Cell is empty.\n\n";
	else std::cout << "Cell is not empty.\n\n";
}

/**
 *Method informs about object in specific cell.
 */
void Tui::PresentCellObject(const std::shared_ptr<CellObject> object) {
	std::cout << "ID: " << object->id_ << std::endl;
	std::cout << "Coordiantes: x = " << object->x_ << ", y = " << object->y_ << ".\n";
}

/**
 *Method informs about object in specific cell.
 */
void Tui::PresentCellObject(const std::shared_ptr<MovableObject> object) {
	std::cout << "Presenting moveable object:\n";
	PresentCellObject(static_cast<std::shared_ptr<CellObject>>(object));
	std::cout << "Move priority: " << object->GetMovePriority() << "\n\n";
}
`
/**
 *Method informs about object in specific cell.
 */
void Tui::PresentCellObject(const std::shared_ptr<NonMovableObject> object) {
	std::cout << "Presenting non-moveable object:\n";
	PresentCellObject(static_cast<std::shared_ptr<CellObject>>(object));
	std::cout << std::endl;
}


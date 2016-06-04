#include "Field.hpp"
#include "FieldCell.hpp"
#include "CellObject.hpp"
#include "Tui.hpp"
#include <iostream>

void Tui::DrawHorizontalLine() {
	const std::shared_ptr<Field> field = Field::GetInstance();
	for (size_t i = 0; i <= field->GetWidth() * 2; i++)
		std::cout << "-";
	std::cout << std::endl;
}

void Tui::PrintField() {
//	const std::shared_ptr<Field> field = Field::GetInstance();
	DrawHorizontalLine();
	try {
	for (size_t j = 0; j < field->GetHeight(); j++) {
		for (size_t i = 0; i < field->GetWidth(); i++) {
			if (field->GetCell(i, j)->IsEmpty()) {
				std::cout << "  ";
			}
			else {
			//	std::weak_ptr<CellObject> object = field->GetCell(i, j)->GetObject();
			//	CellObject *p = object.lock().get();
				auto p = field->GetCell(i, j)->GetObject();
				if (p->GetType(CellObject::Type::UNIT)) {
					if (p->GetType(CellObject::Type::HERBIVORE)) std::cout << "VV";
					else if (p->GetType(CellObject::Type::CARNIVORE)) std::cout << "CC";
					else std::cout << "UU";
/*					size_t id = p->GetId();
					while (id >= 100) id -= 100;
					if (id < 10) std::cout << "0";
					std::cout << id;	/**/
				}
				else if (p->GetType(CellObject::Type::TREE)) std::cout << "!!";
				else if (p->GetType(CellObject::Type::FLESH)) std::cout << "##";
				else std::cout << "--";
			}
		}
		std::cout << "|\n";
	}
	}
	catch (std::exception &e) {
		std::cout << e.what() << "\n\n";
		return;
	}
	DrawHorizontalLine();
}
/*
void Tui::PresentFieldCell(const size_t x, const size_t y) {
	std::cout << "Presenting cell:\n";
	std::cout << "Coordinates: x = " << x << ", " << y << ".\n";
	if (Field::GetInstance()->GetCell(x, y)->IsEmpty()) std::cout << "Cell is empty.\n\n";
	else std::cout << "Cell is not empty.\n\n";
}

void Tui::PresentCellObject(const std::shared_ptr<CellObject> object) {
	std::cout << "ID: " << object->GetId() << std::endl;
	std::cout << "Coordiantes: x = " << object->GetX() << ", y = " << object->GetY() << ".\n";
}

void Tui::PresentCellObject(const std::shared_ptr<Unit> object) {
	std::cout << "Presenting moveable object:\n";
	PresentCellObject(std::static_pointer_cast<CellObject>(object));
	std::cout << "Move priority: " << object->GetMovePriority() << "\n\n";
}

void Tui::PresentCellObject(const std::shared_ptr<NonMovableObject> object) {
	std::cout << "Presenting non-moveable object:\n";
	PresentCellObject(static_cast<std::shared_ptr<CellObject>>(object));
	std::cout << std::endl;
}
/**/

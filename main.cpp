#include "Field.hpp"

int main(void) {
	Field &field = Field::getInstance(10, 10);
	FieldCell &cell = field.getCell(4, 4);
	cell.getGroundType();
}


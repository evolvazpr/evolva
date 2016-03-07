#include "CellObject.hpp"

CellObject::~CellObject(){}

bool MovableObject::MoveWithRelativeStep(int x, int y) {
	return x == y;
}

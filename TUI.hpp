#ifndef _TUI_H_
#define _TUI_H_
#include <iostream>
#include <memory>

#include "Field.hpp"
#include "FieldCell.hpp"
#include "CellObject.hpp"

class TUI {
private:
	void PresentCellObject(std::shared_ptr<CellObject> object);
public:
	void PrintMap();
	void PresentFieldCell(std::shared_ptr<FieldCell> cell);
	void PresentMovableObject(std::shared_ptr<MovableObject> object);
	void PresentNonMovableObject(std::shared_ptr<NonMovableObject> object);
};

#endif //_TUI_H_

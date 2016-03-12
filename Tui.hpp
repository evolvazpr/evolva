#ifndef _TUI_H_
#define _TUI_H_

#include <boost/multi_array.hpp>

#include <iostream>
#include <memory>

#include "Field.hpp"
#include "FieldCell.hpp"
#include "CellObject.hpp"

class Tui {
private:
	void PresentCellObject(const std::shared_ptr<CellObject> object);
	void DrawHorizontalLine();
public:
	void PrintField();
	void PresentFieldCell(const size_t x, const size_t y);
	void PresentCellObject(const std::shared_ptr<MovableObject> object);
	void PresentCellObject(const std::shared_ptr<NonMovableObject> object);
};

#endif //_TUI_H_

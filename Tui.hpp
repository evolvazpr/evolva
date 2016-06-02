#ifndef _TUI_H_
#define _TUI_H_

#include <memory>

class CellObject;
class Unit;
class NonMovableObject;

class Tui {
private:
//	void PresentCellObject(const std::shared_ptr<CellObject> object);
	void DrawHorizontalLine();
public:
	void PrintField();/*
	void PresentFieldCell(const size_t x, const size_t y);
	void PresentCellObject(const std::shared_ptr<Unit> object);
	void PresentCellObject(const std::shared_ptr<NonMovableObject> object);/**/
};

#endif //_TUI_H_

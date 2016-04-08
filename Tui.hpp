#ifndef _TUI_H_
#define _TUI_H_

#include <memory>

class CellObject;
class MovableObject;
class NonMovableObject;
/**\brief Text user interface*/
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

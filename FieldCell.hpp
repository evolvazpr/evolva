#ifndef _FIELD_CELL_HPP_
#define _FIELD_CELL_HPP_

#include <memory>

class CellObject;

class FieldCell {
private:
	std::shared_ptr<CellObject> object_;
public:
	FieldCell();
	virtual ~FieldCell();
	std::shared_ptr<CellObject> SetObject(std::shared_ptr<CellObject> object);
	inline std::shared_ptr<CellObject> RemoveObject() { return SetObject(nullptr); };
	inline bool IsEmpty() const { return static_cast<bool>(object_); };
};

#endif // _FIELD_CELL_HPP_

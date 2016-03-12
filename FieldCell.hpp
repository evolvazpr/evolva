#ifndef _FIELD_CELL_HPP_
#define _FIELD_CELL_HPP_

#include <memory>

class CellObject;
class Field;

class FieldCell {
	friend class Field; //what for? I don't remember.
private:
	std::shared_ptr<CellObject> object_;
public:
	FieldCell() { object_ = nullptr; };
	std::shared_ptr<CellObject> SetObject(std::shared_ptr<CellObject> object);
	inline std::shared_ptr<CellObject> CopyPtr() { return object_; } //Need by TUI for recognition of object's type
	inline std::shared_ptr<CellObject> RemoveObject() { return SetObject(nullptr); };
	inline bool IsEmpty() const { return (object_ == nullptr); };
};

#endif // _FIELD_CELL_HPP_

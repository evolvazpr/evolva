#ifndef _CELL_OBJECT_H_
#define _CELL_OBJECT_H_

#include <memory>

// forward declarations
class Field;

class CellObject : public std::enable_shared_from_this<CellObject> {
public:
	enum class Type : char { MOVABLE, NONMOVABLE }; //enum describes type of object. Right now used only by TUI.
	CellObject();
	CellObject(const size_t id);
	inline size_t GetId() const { return id_; }
	virtual CellObject::Type GetType() = 0;				//Virtual method, returns type of object.
	virtual ~CellObject() = 0;
	friend class Field;	//Allow Field to change object's x, y coordinates
	friend class Tui;
private:
	const size_t id_;		//Unique object's identifiacation number
	size_t x_;					//Object's x field coordinate
	size_t y_;					//Object's y field coordinate
};

class MovableObject : public CellObject {
friend class Tui;
public:
	MovableObject();
	bool MoveWithRelativeStep(int x, int y);
	inline bool IsRemoved() const { return removed_; };
	virtual ~MovableObject();
	virtual CellObject::Type GetType() { return Type::MOVABLE; };
	virtual double GetMovePriority() const;
	/*< Operator <= uses for sorting. It is used in custom class CyclingQueue.
	It should be operator < (like in other sorting algorithms), but operator <=
	provides new elements to be inserted AFTER old elements with the same comparing value. */
	inline bool operator <= (const MovableObject &object) const { return GetMovePriority() <= object.GetMovePriority(); };
protected:
	bool removed_;
};

class NonMovableObject : public CellObject {
public:
	NonMovableObject();
	virtual ~NonMovableObject();
	virtual CellObject::Type GetType() { return Type::NONMOVABLE; }
};

class Plant : public NonMovableObject {
};

class NonPlant : public MovableObject {
public:
	NonPlant();
};

class Flesh : public NonMovableObject {
};

#endif //_CELL_OBJECT_H_

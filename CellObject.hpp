#ifndef _CELLOBJECT_H_
#define _CELLOBJECT_H_

#include <iostream>
#include <memory>

class Field;

class CellObject : public std::enable_shared_from_this<CellObject> {
	private:
		const size_t id_;		//Unique object's identifiacation number
		size_t x_;					//Object's x field coordinate
		size_t y_;					//Object's y field coordinate
	public:
		enum class Type : char {MOVABLE, NONMOVABLE}; //enum describes type of object. Right now used only by TUI. 

		CellObject(const size_t id);
		inline size_t GetId() const { return id_; }
		virtual CellObject::Type GetType() = 0;				//Virtual method, returns type of object. 
		virtual ~CellObject() = 0;
		friend class Field;	//Allow Field to change object's x, y coordinates
		friend class Tui;
};

class MovableObject : public CellObject {
private:
	size_t move_priority_;
public:
	MovableObject(const size_t id, const size_t move_priority);
	bool MoveWithRelativeStep(int x, int y);
	virtual ~MovableObject();
	virtual CellObject::Type GetType() { return Type::MOVABLE; }
	friend class Tui;
};

class NonMovableObject : public CellObject {
public:
	NonMovableObject(const size_t id);
	virtual ~NonMovableObject();
	virtual CellObject::Type GetType() { return Type::NONMOVABLE; }
};

#include "Field.hpp" /* Include on bottom of header due to problem with Field::MoveObject inline method:
												definition of inline Field::MoveObject needs complete CellObject declaration */

#endif //_CELLOBJECT_H_

#ifndef _CELLOBJECT_H_
#define _CELLOBJECT_H_

#include <iostream>
#include <utility>

#include "Field.hpp"

class CellObject : public std::enable_shared_from_this<CellObject> {
	private:
		const size_t id_;
		size_t x_;
		size_t y_;
	public:
		CellObject(const size_t id);
		inline size_t GetId() const { return id_; }
		virtual ~CellObject() = 0;

		friend class Field;
		friend class TUI;
};

class MovableObject : public CellObject {
private:
	size_t move_priority_;
public:
	MovableObject(const size_t id, const size_t move_priority);
	bool MoveWithRelativeStep(int x, int y);
	virtual ~MovableObject();
	friend class Field;
	friend class TUI;
};

class NonMovableObject : public CellObject {
public:
	NonMovableObject(const size_t id);
	virtual ~NonMovableObject();
};

#endif //_CELLOBJECT_H_

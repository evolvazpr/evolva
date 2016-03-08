#ifndef _CELLOBJECT_H_
#define _CELLOBJECT_H_

#include <iostream>
#include <utility>

#include "Field.hpp"

class CellObject : public std::enable_shared_from_this<CellObject> {
	private:
		const int id_;
	public:
		CellObject(int id);
		int GetId();
		virtual ~CellObject() = 0;	
};

class MovableObject : public CellObject {
	private:
	int move_priority_;

	public:
		MovableObject(int id, int move_priority);
		virtual ~MovableObject();
		bool MoveWithRelativeStep(int x, int y);
		void Think(){} //Example, this should be virtual void
};

class NonMovableObject : public CellObject {
	public:
		virtual ~NonMovableObject();
		NonMovableObject(int id);
};

#endif //_CELLOBJECT_H_

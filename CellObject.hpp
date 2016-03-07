#ifndef _CELLOBJECT_H_
#define _CELLOBJECT_H_

#include <iostream>
#include <utility>

class CellObject {
	private:
		const int id_;
	public:
		CellObject(int id) : id_(id){}
		int GetId(){ return id_; }
		virtual ~CellObject() = 0;	
};

class MovableObject : public CellObject {
	private:
	int move_priority_;

	public:
		MovableObject(int id, int move_priority) 
			: CellObject(id), move_priority_(move_priority) {};
		virtual ~MovableObject(){}

		bool MoveWithRelativeStep(int x, int y);
};

class NonMovableObject : public CellObject {
	public:
		virtual ~NonMovableObject(){}
		NonMovableObject(int id) : CellObject(id) {};
};

#endif //_CELLOBJECT_H_

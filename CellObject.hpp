#ifndef _CELLOBJECT_H_
#define _CELLOBJECT_H_

#include <iostream>
#include <utility>

#include "Field.hpp"



class RealCoordinates {
	private:
		int x_;
		int y_;
	
	public:
		RealCoordinates(int x, int y) : x_ (x), y_(y) {}
		virtual ~RealCoordinates() = 0;
		
		int get_real_x() { return x_; }
		int get_real_y() { return y_; }
		void get_real_coordinates(int *x, int *y) { *x = x_; *y = y_; }

		void set_real_x(int x) { x_ = x; }
		void set_real_y(int y) { y_ = y; }
	  void set_real_coordinates(std::pair<int, int>& coords) { x_ = coords.first; y_ = coords.second; };
};

class CellObject : protected RealCoordinates {
	public:
		CellObject(int x, int y) : RealCoordinates(x, y) {};
		virtual ~CellObject() = 0;	
};

class MovableObject : public CellObject {
	private:
	int move_priority_;

	public:
		MovableObject(int x, int y, int move_priority) 
			: CellObject(x, y), move_priority_(move_priority) {};
		virtual ~MovableObject() = 0;

		bool MoveWithRelativeStep(int x, int y);
		
		bool operator <(const MovableObject& rhs) {
			return move_priority_ < rhs.move_priority_;
		}	
};

class NonMovableObject : public CellObject {
	public:
		virtual ~NonMovableObject();
		NonMovableObject(int x, int y) : CellObject(x, y) {};
};

#endif //_CELLOBJECT_H_

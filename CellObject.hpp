#ifndef _CELLOBJECT_H_
#define _CELLOBJECT_H_

#include <iostream>


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


		virtual void set_real_x(int x) { x_ = x; }
		virtual void set_real_y(int y) { y_ = y; }
		virtual void set_real_coordinates(int x, int y) { x_ = x; y_ = y; };
};

class CellObject : public RealCoordinates {
	public:
		CellObject(int x, int y) : RealCoordinates(x, y) {};
		virtual bool is_movable() = 0;
		virtual ~CellObject(){};	
};

class MovableObject : public CellObject {

};

class NonMovableObject : public CellObject {

};


#endif //_CELLOBJECT_H_

#ifndef _FIELD_CELL_HPP_
#define _FIELD_CELL_HPP_

#include <iostream>
#include <memory>

enum GroundType {Sand, Grass, Water};

class Object {
};

class FieldCell {
	private:
		Object object_;
		GroundType ground_type_;
  public:
		FieldCell(GroundType g_type = Sand) : ground_type_(g_type){}
		GroundType getGroundType();
};

#endif /* _FIELD_CELL_HPP_ */

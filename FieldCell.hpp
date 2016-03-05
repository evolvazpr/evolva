#ifndef _FIELD_CELL_HPP_
#define _FIELD_CELL_HPP_

#include <iostream>
#include <memory>

#include "CellObject.hpp"

enum GroundType {Sand, Grass, Water};

/*
 * Field cell class
 * bool insertObject(std::shared_ptr<Object> obj):
 *		Method inserts object in cell.
 *		RETURN VALUE:
 *			 On success returns true.
 *			 On failure (when there is already an object in cell) returns false.
 *
 *void removeObject():
 *		Method removes object from cell.
 *
 *GroundType getGroundType():
 *		Returns type of ground.
 */ 

class FieldCell {
	private:
		std::weak_ptr<CellObject> object_;
		GroundType ground_type_;
  public:
		FieldCell(GroundType g_type = Sand) : ground_type_(g_type){}
		bool insertObject(std::shared_ptr<CellObject> obj);
		void removeObject();
		GroundType getGroundType();
};

#endif /* _FIELD_CELL_HPP_ */

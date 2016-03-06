#ifndef _FIELD_CELL_HPP_
#define _FIELD_CELL_HPP_

#include <iostream>
#include <memory>

#include "CellObject.hpp"

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
		size_t ground_type_;
  public:
		FieldCell(const FieldCell& field_cell) 
			: object_(field_cell.object_), ground_type_(field_cell.ground_type_) {}
		FieldCell(size_t g_type = 0) : ground_type_(g_type){}
		bool InsertObject(std::shared_ptr<CellObject> obj);
		void RemoveObject();
		size_t get_ground_type() { return ground_type_; }
};

#endif /* _FIELD_CELL_HPP_ */

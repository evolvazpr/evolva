#ifndef _FIELD_CELL_HPP_
#define _FIELD_CELL_HPP_

#include <iostream>
#include <memory>

#include "CellObject.hpp"

class FieldCell {
	private:
		std::weak_ptr<CellObject> object_;
		size_t ground_type_;
  public:
		FieldCell(size_t g_type = 0) : ground_type_(g_type){}
		bool insertObject(std::shared_ptr<CellObject> obj);
		void removeObject();
		size_t getGroundType();
		~FieldCell() { std::cout << "~FieldCell called\n"; }
};

#endif /* _FIELD_CELL_HPP_ */

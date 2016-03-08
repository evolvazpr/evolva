#ifndef _FIELD_CELL_HPP_
#define _FIELD_CELL_HPP_

#include <iostream>
#include <memory>

#include "CellObject.hpp"

/*\
 * Class declaration solves problem with recursive header include.
 */

class CellObject;

class FieldCell {
	private:
		std::weak_ptr<CellObject> object_;

  public:
		FieldCell(){}
		bool InsertObject(std::shared_ptr<CellObject> obj);
		void RemoveObject();
		std::weak_ptr<CellObject> CopyObject();
		bool IsEmpty();
		~FieldCell() {}
};

#endif /* _FIELD_CELL_HPP_ */

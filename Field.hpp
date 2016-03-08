#ifndef _FIELD_HPP_
#define _FIELD_HPP_

#include <boost/multi_array.hpp>
#include <utility>
#include <memory>
#include <list>
#include <queue>
#include <map>

#include "FieldCell.hpp"

/*
 * Class declarations solve problem with recursive header include.
 */

class FieldCell;
class MovableObject;
class NonMovableObject;
class CellObject;


class Field {
  private:
	
		const int x_max_;
		const	int y_max_;
	
		boost::multi_array<std::shared_ptr<FieldCell>, 2> cells_;	
		
		std::map<int, std::pair<int, int>> movable_objects_;

	 	static std::shared_ptr<Field> instance_;

		Field(const int x, const int y);
		Field() = delete;
		Field(const Field&) = delete;
		Field& operator=(const Field &) = delete;

		public:
		static std::shared_ptr<Field> GetInstance(const int x = 0, const int y = 0);

		std::weak_ptr<FieldCell> GetCell(int x_real, int y_real); 
		bool InsertObject(std::shared_ptr<MovableObject> object, int x_real, int y_real);
		bool InsertObject(std::shared_ptr<NonMovableObject> object, int x_real, int y_real);
		bool MoveObject(std::shared_ptr<MovableObject> object, int x_steps, int y_steps);

		void IterateOverMovableObjects();
};

#endif /* _FIELD_HPP_ */


#ifndef _FIELD_HPP_
#define _FIELD_HPP_

#include <boost/multi_array.hpp>
#include <utility>
#include <memory>
#include <list>
#include <queue>

#include "FieldCell.hpp"
#include "CellObject.hpp"

class Field {
  private:
	
		const int x_max_;
		const	int y_max_;
	
		boost::multi_array<std::shared_ptr<FieldCell>, 2> cells_;	
	//	std::priority_queue<std::shared_ptr<MovableObject>,
	//		std::list<std::shared_ptr<MovableObject>>, 
	//		std::less<MovableObject> > movable_objects_;
		
		std::list<std::shared_ptr<MovableObject>> movable_objects_;
		std::list<std::shared_ptr<NonMovableObject>> non_movable_objects_;
	 
	 	static std::shared_ptr<Field> instance_;

		Field(const int x, const int y);
		Field() = delete;
		Field(const Field&) = delete;
		Field& operator=(const Field &) = delete;

		public:
		std::weak_ptr<FieldCell> GetCell(int x, int y); 
		static Field& GetInstance(const int x = 0, const int y = 0);

};

#endif /* _FIELD_HPP_ */


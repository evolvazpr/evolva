#ifndef _FIELD_HPP_
#define _FIELD_HPP_

#include <queue>

#include <memory>
#include <vector>
#include <map>

#include <boost/multi_array.hpp>
#include "FieldCell.hpp"
#include "CellObject.hpp"

class Field {
  private:

		const int x_max_;
		const	int y_max_;
		
		boost::multi_array<std::shared_ptr<FieldCell>, 2> cells_;	
		//std::priority_queue<std::shared_ptr<CellObject>,
		//	std::list<std::shared_ptr<CellObject>>, XXX> movable_objects_;
		std::list<std::shared_ptr<CellObject>> non_movable_objects_;

		std::map<std::shared_ptr<CellObject>, std::pair<int, int>> map_;

    static std::shared_ptr<Field> instance_;

		Field(const int x, const int y);
		Field() = delete;
		Field(const Field&) = delete;
		Field& operator=(const Field &) = delete;

		/*
		 * Method changes real coordinates of object in map_ contener.
		 */	
		void CalculateNewPositionOfObject(std::shared_ptr<CellObject> object, int x_relative, int y_relative);		
	
		/*
		 * Method returns cell related to (x, y) real cooridantes.
		 */
		std::weak_ptr<FieldCell> GetCell(int x, int y); 
		
		/*
		 * Method returns cell related to coordinates wich are in relation to given object.
		 * Method searches for object's coordinates in map_ contener, then adds x_relative and y_relative to real object's coordinates
		 * and uses getCell method to retrive cell.
		 */
		std::weak_ptr<FieldCell> GetCellWithRelativeCoords(std::shared_ptr<CellObject> object, 
																											 int x_relative, int y_relative);


	public:

		/*
		 * Singleton method, which is used to get instace of Field.
		 */
		static Field& GetInstance(const int x = 0, const int y = 0);

		/*
		 * Method changes coordinates of object in map_ contener.
		 * Method seraches for object's coordinates in map_ contener, calls GetCell twice: for present object's cell, and for future cell.
		 * Inserts object in future cell (if it is possible. If not, method return false), removes object from present cell, updates object's
		 * coordinates in map_ contener. 
		 */
		bool MoveObjectWithRelativeStep(std::shared_ptr<CellObject> object, int x_relative, int y_relative);
	
		/*
		 * Method adds object to world:
		 * Inserts object in cell (if it is possible. If not, returns false), updates object's coordinates in map_ contener, updates all important
		 * data structures.
		 */
		bool AddObject(std::shared_ptr<CellObject> object, int x, int y);
		
		/*
		 * Method removes object from world.
		 */
		bool RemoveObject(std::shared_ptr<CellObject> object);
};

#endif /* _FIELD_HPP_ */


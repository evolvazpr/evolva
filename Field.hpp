#ifndef _FIELD_HPP_
#define _FIELD_HPP_


#include <memory>
#include <vector>

#include <boost/multi_array.hpp>
#include "FieldCell.hpp"
#include "CellObject.hpp"

/*
 * class Field - represents "world".
 *
 * Public methods:
 * std::shared_ptr<FieldCell> getCell(int x, int y):
 *		Parameters: 
 *			x, y coordinates of Field.
 *		Return value:
 *			Method returns shared pointer to FieldCell object located at (x, y) or nullptr, if x or y does not 
 *			exceed field limits.
 
 * std::shared_ptr<FieldCell> getCellWithRelativeCoords(std::shared_ptr<CellObject> object, int x_relative,
 * int y_relative):
 *		Parameters:
 *			object - object which is the reference for x and y relative cooridantes.
 *			x_relative, y_relative - relative coordinates.
 *		Return value:
 *			Method returns shared pointer to FieldCell cell located at x_relative, y_relative coordinates, 
 *			which are in relation to first parameter - object.
 *
 * bool moveObjectWithRelativeStep(std::shared_ptr<CellObject> object, int x_relative, int y_relative):
 *		Method moves movable object.
 *		Parameters:
 *			object - object to move.
 *			x_relative, y_relative - relative coordinates of final position in relation to initial position.
 *		Return value:
 *			True on success, false on fail. Failure is possible when cell is already occupied, or object not
 *			movable.
 *
 * bool addObjectToWorld(std::shared_ptr<CellObject> object, int x, int y)
 *		adds ListElement to std::vector<std::shared_ptr<ListElement>> object_list_ and updates proper 
 *		FieldCell if this cell is not occupied yet.
 *		Parameters:
 *			object - pointer to object which will be inserted.
 *			x and	y - location of object on field.
 *		Return value:
 *			True on success, false on fail. Failure is possible when location on field is already occupied.
 *
 */

class Field {
  private:

		struct ListElement {
			std::shared_ptr<CellObject> object_;
			int x_physical_;
			int y_physical_;

			ListElement(std::shared_ptr<CellObject> object, int x, int y);
		};

		const int x_max_;
		const	int y_max_;
		
		boost::multi_array<std::shared_ptr<FieldCell>, 2> cells_;		
		std::vector<std::shared_ptr<ListElement>> object_list_;		

    static std::shared_ptr<Field> instance_;

		Field(const int x, const int y);
		Field() = delete;
		Field(const Field&) = delete;
		Field & operator=(const Field &) = delete;
	
		void calculateNewPositionOfObject(std::shared_ptr<ListElement> list_element, int x_relative, int y_relative);		
	public:

		static Field & getInstance(const int x = 0, const int y = 0);

		std::shared_ptr<FieldCell> getCell(int x, int y); 
		std::shared_ptr<FieldCell> getCellWithRelativeCoords(std::shared_ptr<CellObject> object, int x_relative,
																											 int y_relative);

		bool moveObjectWithRelativeStep(std::shared_ptr<CellObject> object, int x_relative, int y_relative);
		bool addObjectToWorld(std::shared_ptr<CellObject> object, int x, int y);
		bool removeObjectFromWorld(std::shared_ptr<CellObject> object);
};

#endif /* _FIELD_HPP_ */


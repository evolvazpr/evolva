#ifndef _FIELD_HPP_
#define _FIELD_HPP_

#include <memory>
#include <vector>

#include "FieldCell.hpp"
#include "CellObject.hpp"

/*
 * TODO:
 * think!
 */

/*
 * Klasa Field - klasa reprezentująca świat.
 */

class Field {
  private:
		struct listElement {
			std::shared_ptr<CellObject> object_;
			int x_physical_;
			int y_physical_;

			listElement(std::shared_ptr<CellObject> object, int x, int y) : object_(object), x_physical_(x), y_physical_(y){}
		};

		const int x_max_;
		const	int y_max_;
		
		std::vector<std::shared_ptr<FieldCell>> cells_;						//TODO: is it oks?
		std::vector<std::shared_ptr<listElement>> object_list_;		//TODO: is it oks?

    static std::shared_ptr<Field> instance_;

		Field(const int x, const int y);
		Field() = delete;
		Field(const Field&) = delete;
		Field & operator=(const Field &) = delete;
		void calculateNewPositionOfObject(std::shared_ptr<listElement> list_element, int x_relative, int y_relative);	
	public:

		static Field & getInstance(const int x = 0, const int y = 0);
		std::shared_ptr<FieldCell> getCell(int x, int y); 
		std::shared_ptr<FieldCell> getCellWithRelativeCoords(std::shared_ptr<CellObject> object, int x_relative, int y_relative);
		bool moveObjectWithRelativeStep(std::shared_ptr<CellObject> object, int x_relative, int y_relative);
		
		/*
		 * TODO: REMOVE IT!
		 */
		void addObjectToVector(std::shared_ptr<CellObject> object, int x, int y) {
			object_list_.push_back(std::shared_ptr<listElement>(new listElement(object, x, y)));
		}
};

#endif /* _FIELD_HPP_ */


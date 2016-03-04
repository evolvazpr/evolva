#ifndef _FIELD_HPP_
#define _FIELD_HPP_

#include <memory>

#include "FieldCell.hpp"

class Field {
  private:
	
		const int x_max_;
		const	int y_max_;
		const std::unique_ptr<FieldCell[]> cells_;

    static Field * instance_;

		Field(const int x, const int y);
		Field() = delete;
		Field(const Field&) = delete;
		Field & operator=(const Field &) = delete;
	
	public:
	
		static Field & getInstance(const int x = 0, const int y = 0);
		FieldCell & getCell(int x, int y); 

};

#endif /* _FIELD_HPP_ */


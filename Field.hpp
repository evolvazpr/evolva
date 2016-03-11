#ifndef _FIELD_HPP_
#define _FIELD_HPP_

#include <boost/multi_array.hpp>
#include <memory>
#include <list>

#include "FieldCell.hpp"
#include "CellObject.hpp"
#include "TUI.hpp"

class CellObject;
class MovableObject;
class NonMovableObject;
class FieldCell;

class Field {

	friend class TUI;

private:
	const size_t width_; //ilosc xow
	const size_t height_; //ilosc ykow

	boost::multi_array<std::shared_ptr<FieldCell>, 2> cells_;	
	
	std::list<std::shared_ptr<MovableObject>> moveable_objects_;
	std::vector<std::shared_ptr<NonMovableObject>> non_movable_objects_;

	static std::shared_ptr<Field> instance_;

	Field(const size_t x, const size_t y);
	Field() = delete;
	Field(const Field&) = delete;
	Field& operator=(const Field &) = delete;

public:
	static std::shared_ptr<Field> GetInstance(const size_t x = 0, const size_t y = 0);
	inline bool IsCorrect(const size_t x, const size_t y) const ;
	inline std::shared_ptr<FieldCell> GetCell(const size_t x, const size_t y);
	bool InsertObject(std::shared_ptr<CellObject> object, const size_t x, const size_t y);
	bool InsertObject(std::shared_ptr<MovableObject> object, const size_t x, const size_t y);
	bool InsertObject(std::shared_ptr<NonMovableObject> object, const size_t x, const size_t y);
	bool MoveObject(std::shared_ptr<MovableObject> object, const long x_steps, const long y_steps);
	bool MoveObjectTo(std::shared_ptr<MovableObject> object, const size_t x, const size_t y);
};

#endif // _FIELD_HPP_


#ifndef _FIELD_CELL_HPP_
#define _FIELD_CELL_HPP_

// Includes
#include <memory>

// Forward declarations
class CellObject;
class Field;
class NonMovableObject;

class FieldCell : public std::enable_shared_from_this<FieldCell> {
friend class Field;
public:
	FieldCell() = delete;
	FieldCell(const size_t x, const size_t y);
	virtual ~FieldCell();
	std::shared_ptr<CellObject> SetObject(std::shared_ptr<CellObject> object);
	inline std::shared_ptr<CellObject> GetObject() { return object_; };
	std::shared_ptr<NonMovableObject> GetNmo(); //UNSAFE :(
	inline std::shared_ptr<CellObject> RemoveObject() { return SetObject(nullptr); };
	inline bool IsEmpty() const { return (object_ == nullptr); };
	inline size_t GetX() const { return x_; };
	inline size_t GetY() const { return y_; };
	enum class Ground : unsigned char {
		GROUND = 0,
		WATER,
		GRASS,
		ROCK
	};
private:
	std::shared_ptr<CellObject> object_;
	Ground ground_;
	size_t x_;
	size_t y_;
};

#endif // _FIELD_CELL_HPP_

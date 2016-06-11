#ifndef _FIELD_CELL_HPP_
#define _FIELD_CELL_HPP_

// Includes
#include <memory>

// Forward declarations
class CellObject;
class Field;
class NonMovableObject;
class Unit;

class FieldCell : public std::enable_shared_from_this<FieldCell> {
friend class Field;
public:
	FieldCell() = delete;
	FieldCell(const size_t x, const size_t y);
	virtual ~FieldCell();
	bool SetObject(std::shared_ptr<CellObject> object);
	inline std::shared_ptr<CellObject> GetObject() { return object_; };
	std::shared_ptr<Unit> GetUnit();
	std::shared_ptr<NonMovableObject> GetNmo(); //UNSAFE :(
	inline bool RemoveObject() { return SetObject(nullptr); };
	inline bool IsEmpty() const { return (object_ == nullptr); };
	inline size_t GetX() const { return x_; };
	inline size_t GetY() const { return y_; };
	enum class Ground : unsigned char {
		GROUND = 0,
		GRASS,
	};
	inline Ground GetGroundType() const { return ground_; };
	inline void SetGroundType(Ground ground) { ground_ = ground; };
private:
	FieldCell(const FieldCell&) = delete;
	FieldCell& operator = (const FieldCell&) = delete;
	std::shared_ptr<CellObject> object_;
	Ground ground_;
	size_t x_;
	size_t y_;
};

#endif // _FIELD_CELL_HPP_

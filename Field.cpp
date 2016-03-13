#include "Field.hpp"
#include "FieldCell.hpp"
#include "CellObject.hpp"
#include "CyclicQueue.hpp"
#include <vector>
#include <boost/multi_array.hpp>


class FieldPimpl {
friend class Field;
private:
	const size_t width_; //ilosc xow
	const size_t height_; //ilosc ykow

	mutable size_t ffid_ = 0;

	boost::multi_array<std::shared_ptr<FieldCell>, 2> cells_;

	CyclicQueue movable_objects_;
	std::vector<std::shared_ptr<NonMovableObject>> non_movable_objects_;

	FieldPimpl(const size_t x, const size_t y);
	FieldPimpl() = delete;
public:
	~FieldPimpl();
};

FieldPimpl::FieldPimpl(const size_t x, const size_t y) : width_(x), height_(y), cells_(boost::extents[x][y]) {
}

FieldPimpl::~FieldPimpl() {
}

std::shared_ptr<Field> Field::instance_ = nullptr;

Field::Field(const size_t x, const size_t y) : pimpl_(new FieldPimpl(x, y)) {
	for (size_t i = 0; i < x; i++) {
		for (size_t j = 0; j < y; j++) {
			pimpl_->cells_[i][j] = std::make_shared<FieldCell>();
		}
	}
}

std::shared_ptr<Field> Field::GetInstance(const size_t x, const size_t y) {
	if (!instance_) {
		instance_ = std::shared_ptr<Field>(new Field(x, y));
	}
	return instance_;
}

size_t Field::GetWidth() const {
	return pimpl_->width_;
}

size_t Field::GetHeight() const {
	return pimpl_->height_;
}

bool Field::InsertObject(std::shared_ptr<CellObject> object, const size_t x, const size_t y) {
	std::shared_ptr<FieldCell> cell = GetCell(x, y);
	if (cell->IsEmpty()){
		cell->SetObject(object);
		object->x_ = x; //the same as in MoveObjectTo????
		object->y_ = y;
		return true;
	}
	else return false;
}

bool Field::InsertObject(std::shared_ptr<MovableObject> object, const size_t x, const size_t y) {
	bool insertion = InsertObject(static_cast<std::shared_ptr<CellObject>>(object), x, y);
	if (insertion){
		pimpl_->movable_objects_.Insortion(object);
		return true;
	}
	else return false;
}

bool Field::InsertObject(std::shared_ptr<NonMovableObject> object, const size_t x, const size_t y) {
	bool insertion = InsertObject(static_cast<std::shared_ptr<CellObject>>(object), x, y);
	if (insertion){
		pimpl_->non_movable_objects_.push_back(object);
		return true;
	}
	else return false;
}

bool Field::MoveObjectTo(std::shared_ptr<MovableObject> object, const size_t x, const size_t y) {
	std::shared_ptr<FieldCell> source_cell = GetCell(object->x_, object->y_);
	if (source_cell->object_ != object) {
		throw EvolvaException("Serious memory problem.");
	}
	if (!IsCorrect(x, y)) return false;
	std::shared_ptr<FieldCell> cell = GetCell(x, y);
	if (!cell->IsEmpty()) return false;
	source_cell->RemoveObject();
	cell->SetObject(object);
	object->x_ = x;
	object->y_ = y;
	//TODO: Gui move
	return true;
}

bool Field::Kill(Unit *unit) {
	return true;
}

bool Field::Kill(std::shared_ptr<Unit> unit) {
	return true;
}


bool Field::IsCorrect(const size_t x, const size_t y) const {
	return (x < pimpl_->width_ && y < pimpl_->height_);
}


std::shared_ptr<FieldCell> Field::GetCell(const size_t x, const size_t y) {
	return pimpl_->cells_[x][y];
}

bool Field::MoveObject(std::shared_ptr<MovableObject> object, const long x_steps, const long y_steps) {
	return MoveObjectTo(object, object->x_ + x_steps, object->y_ + y_steps);
}

bool Field::BeginCycle() {
	if (pimpl_->movable_objects_.empty()) return false;
	pimpl_->movable_objects_.Begin();
	return true;
}

bool Field::Next() {
	return pimpl_->movable_objects_.Next();
}

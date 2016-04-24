#include "Field.hpp"
#include "FieldCell.hpp"
#include "CellObject.hpp"
#define private public
#include "CyclicQueue.hpp"
#undef private
#include "Unit.hpp"
#include <vector>
#include <boost/multi_array.hpp>
#include "dialog.hpp"
#include <iostream>

std::shared_ptr<Field> field;

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
	bool pause_;
	size_t global_turns_;
public:
	~FieldPimpl();
};

FieldPimpl::FieldPimpl(const size_t x, const size_t y) : width_(x), height_(y), cells_(boost::extents[x][y]) {
	pause_ = true;
	global_turns_ = 0;
}

FieldPimpl::~FieldPimpl() {
}

std::shared_ptr<Field> Field::instance_ = nullptr;
Dialog* Field::qt_dialog_ = nullptr;

Field::Field(const size_t x, const size_t y) : pimpl_(new FieldPimpl(x, y)) {
	for (size_t i = 0; i < x; i++) {
		for (size_t j = 0; j < y; j++) {
			pimpl_->cells_[i][j] = std::make_shared<FieldCell>(i, j);
		}
	}
}

std::shared_ptr<Field> Field::GetInstance(const size_t x, const size_t y, Dialog * qt_dialog) {
	if (!instance_) {
		instance_ = std::shared_ptr<Field>(new Field(x, y));
		qt_dialog_ = qt_dialog;
	}
	return instance_;
}

size_t Field::GetWidth() const {
	return pimpl_->width_;
}

size_t Field::GetHeight() const {
	return pimpl_->height_;
}

bool Field::InsertCellObject(std::shared_ptr<CellObject> object, const size_t x, const size_t y) {
	std::shared_ptr<FieldCell> cell = GetCell(x, y);
	if (cell->IsEmpty()){
		cell->SetObject(object);
//		object->cell_->x_ = x; //the same as in MoveObjectTo????
//		object->cell_->y_ = y;
		return true;
	}
/*	else {
		std::cout << "!!!!!!!   " << x << "  " << y << "  !!!!!!!!!!!!\n\n";
		char c;
		std::cin >> c;
		return false;
	}/**/
}

bool Field::InsertObject(std::shared_ptr<MovableObject> object, const size_t x, const size_t y) {
//	auto grr = object.get();
//	auto mhm = std::static_pointer_cast<CellObject>(object).get();
	bool insertion = InsertCellObject(std::static_pointer_cast<CellObject>(object), x, y);
//	bool insertion = true;
	if (insertion){
		std::cout << "xxx\n";
		f1();
		pimpl_->movable_objects_.Insortion(object);
		f1();
		std::cout << "yyy\n";
	/*	{
			auto p1 = pimpl_->movable_objects_.position_;
//			auto p2 = pimpl_->movable_objects_.previous_position_;
//			auto p3 = pimpl_->movable_objects_.pushed_end_;
			auto p4 = pimpl_->movable_objects_.end_;
			pimpl_->movable_objects_.Begin();
			std::cout << "que:\n";
            while (1) {
				std::cout << pimpl_->movable_objects_.Get()->GetId() << "\n";
				if (pimpl_->movable_objects_.IsEnd()) break;
				else pimpl_->movable_objects_.Next();
            }
            std::cout << "\n";
            pimpl_->movable_objects_.position_ = p1;
//            pimpl_->movable_objects_.previous_position_ = p2;
            pimpl_->movable_objects_.end_ = p4;
		}/**/
		
		qt_dialog_->createObject(object->id_, x, y, Qt::red);
		
		return true;
	}
	else return false;
}

bool Field::InsertObject(std::shared_ptr<NonMovableObject> object, const size_t x, const size_t y) {
	bool insertion = InsertCellObject(std::static_pointer_cast<CellObject>(object), x, y);
	if (insertion){
		pimpl_->non_movable_objects_.push_back(object);
		qt_dialog_->createObject(object->id_, x, y, Qt::blue);
		return true;
	}
	else return false;
}

bool Field::MoveObjectTo(std::shared_ptr<MovableObject> object, size_t x, size_t y, const bool trim) {
	std::shared_ptr<FieldCell> source_cell = object->cell_.lock();
	if (source_cell->object_ != object) {
		throw EvolvaException("Serious memory problem.");
	}
//	source_cell.reset();
//	if (!IsCorrect(x, y)){
		if (trim) {
			if (x >= pimpl_->width_) x = pimpl_->width_ - 1;
			if (y >= pimpl_->height_) y = pimpl_->height_ - 1;
		}
		else return false;
//	}
	std::shared_ptr<FieldCell> cell = GetCell(x, y);
	if (!cell->IsEmpty()) return false;
	source_cell->RemoveObject();
	cell->SetObject(object);
//	object->x_ = x;
//	object->y_ = y;
	qt_dialog_->moveObjectTo(object->id_, x, y);	
	return true;
}

bool Field::Kill(std::shared_ptr<Unit> unit) {
	auto cell = GetCell(unit->GetX(), unit->GetY());
	if (cell->GetUnit() == unit) {
		std::cout << unit->GetId() << " is dead\n";
		cell->RemoveObject();
		unit->removed_ = true;
		unit->alive_ = false;		
		qt_dialog_->removeObject(unit->id_);
	}
	else throw; //wow problem memory
}

bool Field::KillNmo(std::shared_ptr<NonMovableObject> object) {
	for (size_t i = 0; i < pimpl_->non_movable_objects_.size(); ++i) {
		if (object ==  pimpl_->non_movable_objects_[i]) {
			GetCell(object->GetX(), object->GetY())->RemoveObject();
			qt_dialog_->removeObject(object->id_);
			if (pimpl_->non_movable_objects_.size() == 1) pimpl_->non_movable_objects_.clear();
			else if (i + 1 ==  pimpl_->non_movable_objects_.size())  pimpl_->non_movable_objects_.pop_back();
			else {
				std::cout << "usuwanie\n";
				std::cout << "x: " << pimpl_->non_movable_objects_[i]->GetX() << " y: " << pimpl_->non_movable_objects_[i]->GetY() << "\n";
				std::cout << "x: " << object->GetX() << " y: " << object->GetY() << "\n";
			//	pimpl_->non_movable_objects_[i].reset();
			//	GetCell(object->GetX(), object->GetY())->RemoveObject();
//				object->GetCell()->RemoveObject();
		//		object.reset();
				pimpl_->non_movable_objects_[i] =  pimpl_->non_movable_objects_.back();
				pimpl_->non_movable_objects_.pop_back();
				std::cout << "x: " << pimpl_->non_movable_objects_[i]->GetX() << " y: " << pimpl_->non_movable_objects_[i]->GetY() << "\n";

			}
			return true;
		}
	}
	return false;
}

bool Field::IsCorrect(const size_t x, const size_t y) const {
	return (x < pimpl_->width_ && y < pimpl_->height_);
}


std::shared_ptr<FieldCell> Field::GetCell(const size_t x, const size_t y) {
	return pimpl_->cells_[x][y];
}

bool Field::MoveObject(std::shared_ptr<MovableObject> object, const long x_steps, const long y_steps, const bool trim) {
	const bool x_trim = (x_steps < 0 && static_cast<size_t>(-x_steps) > object->GetX());
	const bool y_trim = (y_steps < 0 && static_cast<size_t>(-y_steps) > object->GetY());
	if (trim) {
		return MoveObjectTo(object, x_trim ? 0 : object->GetX() + x_steps, y_trim ? 0 : object->GetY() + y_steps, true);
	}
	else return MoveObjectTo(object, object->GetX() + x_steps, object->GetY() + y_steps, trim);
}

bool Field::BeginCycle() {
	if (pimpl_->movable_objects_.empty()) return false;
	pimpl_->movable_objects_.Begin();
	return true;
}

std::shared_ptr<MovableObject> Field::GetCurrentObject() {
	return pimpl_->movable_objects_.Get();
}

std::shared_ptr<Unit> Field::GetCurrentUnit() {
	return std::dynamic_pointer_cast<Unit>(GetCurrentObject());
}

std::shared_ptr<MovableObject> Field::Next() {
	// Take next turn. Then check whether the field is paused. Only non-paused
	// field pass methods such Update() and Think().
	pimpl_->movable_objects_.Next();
	if (pimpl_->movable_objects_.empty()) {
		std::cout << "NIOH!\n";
		return nullptr;
	}
	if (!pimpl_->pause_) {
		// If loop reaches its end, then increment global turn counter,
		// then sort (new turn, new order) and back to square one.
		if (pimpl_->movable_objects_.IsEnd()) {
			pimpl_->global_turns_++;
			pimpl_->movable_objects_.Sort();
			pimpl_->movable_objects_.Begin();
		}
		// First, check is object an unit, then update object (body-related
		// issues, recalculating movement priority. Then, let an unit think.
		if (pimpl_->movable_objects_.Get()->GetType(CellObject::Type::UNIT)) {
			auto unit = std::dynamic_pointer_cast<Unit>(pimpl_->movable_objects_.Get());
			if (unit->GetId() == 9) {
				std::cout << "YOU!\n";
			}
			unit->Update();
//			unit->Think();
		}
	}
	// Method returns next object.
	return field->GetCurrentObject();
}

std::shared_ptr<Unit> Field::NextUnit() {
	return std::dynamic_pointer_cast<Unit>(Next());
}

void Field::Play() {
	pimpl_->pause_ = false;
//	pimpl_->movable_objects_.PopEnd();
}

void Field::Pause() {
	pimpl_->pause_ = true;
//	pimpl_->movable_objects_.PushEnd();
}
/*
bool Field::IsPauseLoop() const {
	return pimpl_->movable_objects_.IsPushedEnd();
}
/**/
size_t Field::GetGlobalTurnCounter() const {
	return pimpl_->global_turns_;
}

bool Field::IsCycleEnd() const {
	return pimpl_->movable_objects_.IsEnd();
}

void Field::f1() {
	std::cout << "cqs: " << pimpl_->movable_objects_.size() << "\n";
}

void Field::f2() {
	auto z = std::dynamic_pointer_cast<Unit>(pimpl_->movable_objects_.Get());
	auto h = z;
	while (1) {
		std::cout << z->GetId() << "\n";
		std::cout << "preg: " << z->pregnant_ << "\n";
		std::cout << "post: " << z->puerperium_ << "\n";
		std::cout << "energy: " << z->energy_ << "\n";
		std::cout << "fat: " << z->fatigue_ << "\n";
		std::cout << "x: " << z->GetX() << "\n";
		std::cout << "y: " << z->GetY() << "\n\n";
		pimpl_->movable_objects_.Next();
		z = std::dynamic_pointer_cast<Unit>(pimpl_->movable_objects_.Get());
		if (z == nullptr) break;
		if (h->IsRemoved()) {
			h = z;
			z = std::dynamic_pointer_cast<Unit>(pimpl_->movable_objects_.Get());
		}
		if (z == h) break;
	}
}
